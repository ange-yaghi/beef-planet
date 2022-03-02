#ifndef BEEF_PLANET_DUST_SPAWNER_H
#define BEEF_PLANET_DUST_SPAWNER_H

#include "game_object.h"

#include "physics_component.h"
#include "space_dust.h"

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

            int m_maxDustParticles;
            int m_activeDustParticles;
            SpaceDust **m_dust;

            float m_cooldown;
            float m_spawnRadius;
            dbasic::ModelAsset *m_model;
    };
} /* namespace bp */

#endif /* BEEF_PLANET_DUST_SPAWNER_H */
