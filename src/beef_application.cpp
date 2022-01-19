#include "../include/beef_application.h"

#include "../include/physical_object.h"
#include "../include/player_object.h"
#include "../include/planet_spawner.h"

bp::BeefApplication::BeefApplication() {
    m_player = nullptr;
}

bp::BeefApplication::~BeefApplication() {
    /* void */
}

void bp::BeefApplication::Initialize(void *instance, ysContextObject::DeviceAPI api) {
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

    m_assetManager.CompileInterchangeFile((assetPath + "/icosphere").c_str(), 1.0f, true);
    m_assetManager.LoadSceneFile((assetPath + "/icosphere").c_str(), true);

    m_shaders.SetCameraMode(dbasic::DefaultShaders::CameraMode::Target);
    m_shaders.SetCameraPosition(ysMath::Constants::Zero);
    m_shaders.SetCameraTarget(ysMath::Constants::Zero);

    m_engine.SetCursorPositionLock(true);
    m_engine.SetCursorHidden(true);

    m_universe.initialize(&m_engine, &m_shaders);

    PlanetSpawner* planet_spawner = m_universe.spawn<PlanetSpawner>();
    planet_spawner->setModel(m_assetManager.GetModelAsset("Icosphere"));

    m_player = m_universe.spawn<PlayerObject>();
    m_player->getPhysicsComponent()->m_transform.SetPosition(ysMath::LoadVector(0.0f, 0.0f, 0.0f));
    m_player->setModel(m_assetManager.GetModelAsset("Icosphere"));

    m_shaders.SetNearClip(1.0f);
    m_shaders.SetFarClip(500.0f);

    m_shaders.SetFogNear(150.0f);
    m_shaders.SetFogFar(400.0f);
    m_shaders.SetFogColor(clearColor);
}

void bp::BeefApplication::Process() {
    m_universe.process(m_engine.GetFrameLength());
}

void bp::BeefApplication::Render() {
    const int screenWidth = m_engine.GetGameWindow()->GetGameWidth();
    const int screenHeight = m_engine.GetGameWindow()->GetGameHeight();

    m_shaders.SetScreenDimensions((float)screenWidth, (float)screenHeight);
    m_shaders.CalculateCamera();

    //m_shaders.SetCameraPosition(ysMath::LoadVector(4.0f, 4.0f, 2.0f));
    //m_shaders.SetCameraUp(ysMath::Constants::ZAxis);

    m_shaders.ResetLights();
    m_shaders.SetAmbientLight(ysMath::GetVector4(ysColor::srgbiToLinear(0x22, 0x00, 0x33)));

    dbasic::Light sun;
    sun.Active = 1;
    sun.Attenuation0 = 0.0f;
    sun.Attenuation1 = 0.0f;
    sun.Color = ysMath::GetVector4(ysMath::Mul(ysColor::srgbiToLinear(0x33, 0x33, 0x77), ysMath::LoadScalar(1.0f)));
    sun.Direction = ysVector4(0.0f, 0.0f, 0.0f, 0.0f);
    sun.FalloffEnabled = 0;
    sun.Position = ysMath::GetVector4(ysMath::LoadVector(0.0, 1000.0, 0.0));
    m_shaders.AddLight(sun);

    dbasic::Light sun2;
    sun2.Active = 1;
    sun2.Attenuation0 = 0.0f;
    sun2.Attenuation1 = 0.0f;
    sun2.Color = ysMath::GetVector4(ysMath::Mul(ysColor::srgbiToLinear(0x77, 0x33, 0x33), ysMath::LoadScalar(1.0f)));
    sun2.Direction = ysVector4(0.0f, 0.0f, 0.0f, 0.0f);
    sun2.FalloffEnabled = 0;
    sun2.Position = ysMath::GetVector4(ysMath::LoadVector(0.0, -1000.0, 0.0));
    m_shaders.AddLight(sun2);

    m_universe.render();

    //m_engine.SetDrawTarget(dbasic::DeltaEngine::DrawTarget::Gui);
    //m_engine.SetObjectTransform(ysMath::TranslationTransform(ysMath::LoadVector(0.0f, 0.0f, 0.0f)));
    //m_engine.SetBaseColor(ysColor::srgbiToLinear(0xFF, 0xFF, 0xFF));
    //m_engine.SetLit(false);
    //m_engine.DrawBox(10.0, 10.0, 0);
    //m_engine.SetDrawTarget(dbasic::DeltaEngine::DrawTarget::Main);

    //DrawDebugScreen();

    //m_engine.ResetBrdfParameters();
    //m_engine.SetBaseColor(ysColor::srgbiToLinear(0xFF, 0xFF, 0xFF));
    //m_engine.SetObjectTransform(ysMath::TranslationTransform(ysMath::LoadVector(0.0f, 0.0f, 0.0f)));
    //m_engine.SetSpecularRoughness(1.0f);
    //m_engine.SetEmission(ysMath::Mul(ysColor::srgbiToLinear(0xAA, 0xAA, 0xFF), ysMath::LoadScalar(0.2f)));
    //m_engine.DrawModel(m_assetManager.GetModelAsset("Icosphere"), 1.0, nullptr);
}

void bp::BeefApplication::Run() {
    while (m_engine.IsOpen() && !m_engine.IsKeyDown(ysKey::Code::Escape)) {
        m_engine.StartFrame();

        Process();
        Render();

        m_engine.EndFrame();
    }
}

void bp::BeefApplication::Destroy() {
    m_shaderSet.Destroy();

    m_assetManager.Destroy();
    m_engine.Destroy();
}
