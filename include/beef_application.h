#ifndef BEEF_APPLICATION_H
#define BEEF_APPLICATION_H

#include "delta.h"
#include "universe.h"
#include "player_object.h"

namespace bp {
    class BeefApplication {
    public:
        BeefApplication();
        ~BeefApplication();

        void Initialize(void *instance, ysContextObject::DeviceAPI api);
        void Run();
        void Destroy();

    protected:
        void Process();
        void Render();

        dbasic::ShaderSet m_shaderSet;
        dbasic::DefaultShaders m_shaders;

        dbasic::DeltaEngine m_engine;
        dbasic::AssetManager m_assetManager;

        PlayerObject* m_player;
        Universe m_universe;

    };
}  /* namespace bp */
#endif /* BEEF_APPLICATION_H */
