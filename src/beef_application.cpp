#include "../include/beef_application.h"

#include "../include/physical_object.h"
#include "../include/player_object.h"
#include "../include/planet_spawner.h"

#include <sstream>
#include <string>

bp::BeefApplication::BeefApplication() {
    m_player = nullptr;

    m_recording = false;
    m_screenResolutionIndex = 0;
    for (int i = 0; i < ScreenResolutionHistoryLength; ++i) {
        m_screenResolution[i][0] = m_screenResolution[i][1] = 0;
    }
}

bp::BeefApplication::~BeefApplication() {
    /* void */
}

void bp::BeefApplication::initialize(void *instance, ysContextObject::DeviceAPI api) {
    dbasic::Path modulePath = dbasic::GetModulePath();
    dbasic::Path confPath = modulePath.Append("delta.conf");

    std::string enginePath = "../dependencies/submodules/delta-studio/engines/basic";
    std::string assetPath = "../assets";
    if (confPath.Exists()) {
        std::fstream confFile(confPath.ToString(), std::ios::in);

        std::getline(confFile, enginePath);
        std::getline(confFile, assetPath);
        enginePath = modulePath.Append(enginePath).ToString();
        assetPath = modulePath.Append(assetPath).ToString();

        confFile.close();
    }

    m_engine.GetConsole()->SetDefaultFontDirectory(enginePath + "/fonts/");

    const std::string shaderPath = enginePath + "/shaders/";

    dbasic::DeltaEngine::GameEngineSettings settings;
    settings.API = api;
    settings.DepthBuffer = true;
    settings.Instance = instance;
    settings.ShaderDirectory = shaderPath.c_str();
    settings.WindowTitle = "Beef Planet";
    settings.WindowPositionX = 200;
    settings.WindowPositionY = 200;
    settings.WindowHeight = 1000;
    settings.WindowWidth = 2000;
    settings.WindowStyle = ysWindow::WindowStyle::Windowed;

    m_engine.CreateGameWindow(settings);

    m_engine.InitializeShaderSet(&m_shaderSet);
    m_engine.InitializeDefaultShaders(&m_shaders, &m_shaderSet);
    m_engine.InitializeConsoleShaders(&m_shaderSet);
    m_engine.SetShaderSet(&m_shaderSet);

    const ysVector clearColor = ysColor::srgbiToLinear(0x22, 0x00, 0x33);
    m_shaders.SetClearColor(clearColor);

    m_assetManager.SetEngine(&m_engine);

    m_assetManager.CompileInterchangeFile((assetPath + "/assets").c_str(), 1.0f, true);
    m_assetManager.LoadSceneFile((assetPath + "/assets").c_str(), true);

    m_shaders.SetCameraMode(dbasic::DefaultShaders::CameraMode::Target);
    m_shaders.SetCameraPosition(ysMath::Constants::Zero);
    m_shaders.SetCameraTarget(ysMath::Constants::Zero);

    m_engine.SetCursorPositionLock(true);
    m_engine.SetCursorHidden(true);

    m_universe.initialize(&m_engine, &m_shaders);

    PlanetSpawner* planet_spawner = m_universe.spawn<PlanetSpawner>();
    planet_spawner->setModel(m_assetManager.GetModelAsset("Planet"));

    m_player = m_universe.spawn<PlayerObject>();
    m_player->getPhysicsComponent()->m_transform.SetPosition(ysMath::LoadVector(0.0f, 0.0f, 0.0f));
    m_player->setModel(m_assetManager.GetModelAsset("Planet"));

    m_shaders.SetNearClip(1.0f);
    m_shaders.SetFarClip(500.0f);

    m_shaders.SetFogNear(150.0f);
    m_shaders.SetFogFar(400.0f);
    m_shaders.SetFogColor(clearColor);

    m_textRenderer.SetEngine(&m_engine);
    m_textRenderer.SetRenderer(m_engine.GetUiRenderer());
    m_textRenderer.SetFont(m_engine.GetConsole()->GetFont());

    m_timescale = 1.0;
}

void bp::BeefApplication::process() {
    float frameLength = m_engine.GetFrameLength();
    if (frameLength > 1.0 / 30.0) frameLength = 1.0 / 30.0;
    m_timescale = ysMath::GetScalar(m_player->getSizeAnimator()->m_target) * 0.2;
    if (m_timescale < 0.1) m_timescale = 0.1;
    frameLength = frameLength * m_timescale;
    m_universe.process(frameLength);
}

void bp::BeefApplication::render() {
    const int screenWidth = m_engine.GetGameWindow()->GetGameWidth();
    const int screenHeight = m_engine.GetGameWindow()->GetGameHeight();

    m_shaders.SetScreenDimensions((float)screenWidth, (float)screenHeight);
    m_shaders.CalculateCamera();

    m_shaders.ResetLights();
    m_shaders.SetAmbientLight(ysMath::GetVector4(ysColor::srgbiToLinear(0x22, 0x00, 0x33)));

    dbasic::Light sunBlue;
    sunBlue.Active = 1;
    sunBlue.Attenuation0 = 0.0f;
    sunBlue.Attenuation1 = 0.0f;
    sunBlue.Color = ysMath::GetVector4(ysMath::Mul(ysColor::srgbiToLinear(0x33, 0x33, 0x77), ysMath::LoadScalar(1.0f)));
    sunBlue.Direction = ysVector4(0.0f, 0.0f, 0.0f, 0.0f);
    sunBlue.FalloffEnabled = 0;
    sunBlue.Position = ysMath::GetVector4(ysMath::LoadVector(0.0, 1000.0, 0.0));
    m_shaders.AddLight(sunBlue);

    dbasic::Light sunRed;
    sunRed.Active = 1;
    sunRed.Attenuation0 = 0.0f;
    sunRed.Attenuation1 = 0.0f;
    sunRed.Color = ysMath::GetVector4(ysMath::Mul(ysColor::srgbiToLinear(0x77, 0x33, 0x33), ysMath::LoadScalar(1.0f)));
    sunRed.Direction = ysVector4(0.0f, 0.0f, 0.0f, 0.0f);
    sunRed.FalloffEnabled = 0;
    sunRed.Position = ysMath::GetVector4(ysMath::LoadVector(0.0, -1000.0, 0.0));
    m_shaders.AddLight(sunRed);

    std::stringstream ss;
    ss << "FPS: " << m_engine.GetAverageFramerate() << "\n";
    ss << "Size: " << m_player->getSize() << "\n";
    ss << "Timescale: " << m_timescale << "\n";
    ss << "Mass: " << m_player->getPhysicsComponent()->getMass() << "\n";
    ss << "Objects: " << m_universe.getGameObjectCount();
    m_textRenderer.RenderText(
        ss.str(),
        50 - m_engine.GetScreenWidth() / 2.0,
        m_engine.GetScreenHeight() / 2.0 - 50,
        32
    );

    m_universe.render();
}

void bp::BeefApplication::startRecording() {
    m_recording = true;

#ifdef BEEF_PLANET_ENABLE_VIDEO_CAPTURE
    atg_dtv::Encoder::VideoSettings settings{};

    // Output filename
    settings.fname = "../workspace/captures/beef_planet_video_capture.mp4";
    settings.inputWidth = m_engine.GetScreenWidth();
    settings.inputHeight = m_engine.GetScreenHeight();
    settings.width = settings.inputWidth;
    settings.height = settings.inputHeight;
    settings.hardwareEncoding = true;
    settings.inputAlpha = true;
    settings.bitRate = 40000000;

    m_encoder.run(settings, 2);
#endif /* BEEF_PLANET_ENABLE_VIDEO_CAPTURE */
}

void bp::BeefApplication::updateScreenSizeStability() {
    m_screenResolution[m_screenResolutionIndex][0] = m_engine.GetScreenWidth();
    m_screenResolution[m_screenResolutionIndex][1] = m_engine.GetScreenHeight();

    m_screenResolutionIndex = (m_screenResolutionIndex + 1) % ScreenResolutionHistoryLength;
}

bool bp::BeefApplication::readyToRecord() {
    const int w = m_screenResolution[0][0];
    const int h = m_screenResolution[0][1];

    if (w <= 0 && h <= 0) return false;
    if ((w % 2) != 0 || (h % 2) != 0) return false;

    for (int i = 1; i < ScreenResolutionHistoryLength; ++i) {
        if (m_screenResolution[i][0] != w) return false;
        if (m_screenResolution[i][1] != h) return false;
    }

    return true;
}

void bp::BeefApplication::stopRecording() {
        m_recording = false;

#ifdef BEEF_PLANET_ENABLE_VIDEO_CAPTURE
    m_encoder.commit();
    m_encoder.stop();
#endif /* BEEF_PLANET_ENABLE_VIDEO_CAPTURE */
}

void bp::BeefApplication::recordFrame() {
#ifdef BEEF_PLANET_ENABLE_VIDEO_CAPTURE
    const int width = m_engine.GetScreenWidth();
    const int height = m_engine.GetScreenHeight();

    atg_dtv::Frame *frame = m_encoder.newFrame(true);
    if (frame != nullptr && m_encoder.getError() == atg_dtv::Encoder::Error::None) {
        m_engine.GetDevice()->ReadRenderTarget(m_engine.GetScreenRenderTarget(), frame->m_rgb);
    }

    m_encoder.submitFrame();
#endif /* BEEF_PLANET_ENABLE_VIDEO_CAPTURE */
}

void bp::BeefApplication::run() {
    while (m_engine.IsOpen() &&
            (!m_engine.IsKeyDown(ysKey::Code::Escape) ||
            !m_engine.GetGameWindow()->IsActive()))
    {
        m_engine.StartFrame();

        if (m_engine.ProcessKeyDown(ysKey::Code::F1) &&
                m_engine.GetGameWindow()->IsActive())
        {
            if (!isRecording() && readyToRecord()) {
                startRecording();
            }
            else if (isRecording()) {
                stopRecording();
            }
        }

        if (m_engine.ProcessKeyDown(ysKey::Code::N1)) {
            if (m_engine.GetGameWindow()->GetWindowStyle() == ysWindow::WindowStyle::Windowed) {
                m_engine.GetGameWindow()->SetWindowStyle(ysWindow::WindowStyle::Fullscreen);
            }
            else {
                m_engine.GetGameWindow()->SetWindowStyle(ysWindow::WindowStyle::Windowed);
            }
        }

        updateScreenSizeStability();
        if (isRecording() && !readyToRecord()) {
            stopRecording();
        }

        process();
        render();

        m_engine.EndFrame();

        if (isRecording()) {
            recordFrame();
        }
    }

    if (isRecording()) {
        stopRecording();
    }
}

void bp::BeefApplication::destroy() {
    m_shaderSet.Destroy();

    m_assetManager.Destroy();
    m_engine.Destroy();
}
