#ifndef BEEF_APPLICATION_H
#define BEEF_APPLICATION_H

#include "delta.h"
#include "universe.h"
#include "player_object.h"
#include "dtv.h"

namespace bp {
    class BeefApplication {
    public:
        BeefApplication();
        ~BeefApplication();

        void initialize(void *instance, ysContextObject::DeviceAPI api);
        void run();
        void destroy();

    protected:
        void process();
        void render();

        dbasic::ShaderSet m_shaderSet;
        dbasic::DefaultShaders m_shaders;

        dbasic::DeltaEngine m_engine;
        dbasic::AssetManager m_assetManager;

        PlayerObject* m_player;
        Universe m_universe;

    protected:
#ifdef BEEF_PLANET_ENABLE_VIDEO_CAPTURE
        atg_dtv::Encoder m_encoder;
#endif /* BEEF_PLANET_ENABLE_VIDEO_CAPTURE */

        void startRecording();
        void updateScreenSizeStability();
        bool readyToRecord();
        void stopRecording();
        void recordFrame();
        bool isRecording() const { return m_recording; }

        static constexpr int ScreenResolutionHistoryLength = 5;
        int m_screenResolution[ScreenResolutionHistoryLength][2];
        int m_screenResolutionIndex;
        bool m_recording;
    };
}  /* namespace bp */
#endif /* BEEF_APPLICATION_H */
