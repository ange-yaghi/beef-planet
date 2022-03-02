#ifndef BEEF_PLANET_DUST_SPAWNER_H
#define BEEF_PLANET_DUST_SPAWNER_H

#include "game_object.h"

#include "physics_component.h"

#include "delta.h"

namespace bp {
    class DustSpawner : public GameObject {
        public:
            DustSpawner();
            virtual ~DustSpawner();

            virtual void initialize(
                    dbasic::DeltaEngine *engine,
                    dbasic::DefaultShaders *shaders,
                    Universe *universe);

            virtual void process(float dt);
            virtual void render();

            void setModel(dbasic::ModelAsset *asset) { m_model = asset; }

        private:
            void createDust();

            float m_cooldown;
            dbasic::ModelAsset *m_model;
    };
} /* namespace bp */

#endif /* BEEF_PLANET_DUST_SPAWNER_H */