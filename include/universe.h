#ifndef BEEF_PLANET_UNIVERSE_H
#define BEEF_PLANET_UNIVERSE_H

#include "game_object.h"
#include "gravity_simulator.h"
#include "player_object.h"

#include "delta.h"

#include <vector>

namespace bp {
    class Universe {
        public:
            Universe();
            ~Universe();

            void initialize(
                    dbasic::DeltaEngine *engine,
                    dbasic::DefaultShaders *shaders);

            void process(float dt);
            void render();

            template<typename T_GameObject>
            T_GameObject *spawn() {
                T_GameObject *newObject = new T_GameObject;
                newObject->initialize(m_engine, m_shaders, this);

                m_spawn_queue.push_back(newObject);

                return newObject;
            }

            GravitySimulator *getGravitySimulator() { return &m_gravity_simulator; }

            void setScale(double scale) { m_scale = scale; }
            double getScale() { return m_scale; }

            void setTimeScale(float scale) { m_timeScale = scale; }
            double getTimeScale() { return m_timeScale; }

            void drawScaleModel(
                    dbasic::ModelAsset *model,
                    ysTexture *texture,
                    const ysMatrix &transform);
            void addScaleLight(dbasic::Light &light);

            int getGameObjectCount() const { return m_game_objects.size(); }

            GameObject *rayCast(const ysVector &d, const ysVector &p0, uint64_t tags);

        public:
            PlayerObject *m_player;

        private:
            void processSpawnQueue();
            void worldDestroyer();
            GravitySimulator m_gravity_simulator;

        private:
            dbasic::DeltaEngine *m_engine;
            dbasic::DefaultShaders *m_shaders;
            std::vector<GameObject *> m_spawn_queue;
            std::vector<GameObject *> m_game_objects;
            double m_scale;
            float m_timeScale;
    };

} /* namespace bp */

#endif /* BEEF_PLANET_UNIVERSE_H */
