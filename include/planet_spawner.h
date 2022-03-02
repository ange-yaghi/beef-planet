#ifndef BEEF_PLANET_PLANET_SPAWNER_H
#define BEEF_PLANET_PLANET_SPAWNER_H

#include "game_object.h"

#include "physics_component.h"

#include "delta.h"

namespace bp {
    class PlanetSpawner : public GameObject {
        public:
            PlanetSpawner();
            virtual ~PlanetSpawner();

            virtual void initialize(
                    dbasic::DeltaEngine *engine,
                    dbasic::DefaultShaders *shaders,
                    Universe *universe);

            virtual void process(float dt);
            virtual void render();

            void setModel(dbasic::ModelAsset *asset) { m_model = asset; }

        private:
            void createPlanet();
            void createPlanetPointSource();

            float m_cooldown;
            dbasic::ModelAsset *m_model;
            float m_spawnRadius;
    };
} /* namespace bp */

#endif /* BEEF_PLANET_PLANET_SPAWNER_H */
