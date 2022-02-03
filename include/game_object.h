#ifndef BEEF_PLANET_GAME_OBJECT_H
#define BEEF_PLANET_GAME_OBJECT_H

#include "delta.h"

namespace bp {
    class Universe;
    class GameObject {
        public:
            GameObject();
            virtual ~GameObject();

            virtual void initialize(
                    dbasic::DeltaEngine *engine,
                    dbasic::DefaultShaders *shaders,
                    Universe *universe);

            unsigned int getId() { return m_id; }
            void setId(unsigned int id) { m_id = id; }

            virtual void process(float dt);
            virtual void render();

            void scheduleDeletion() { m_deleted = true; }
            bool isDeleted() { return m_deleted; }

            virtual void free();

            static ysMatrix lineHelper(ysVector start, ysVector end);

        private:
            unsigned int m_id;
            bool m_deleted;

        protected:
            dbasic::DeltaEngine* m_engine;
            dbasic::DefaultShaders* m_shaders;
            Universe* m_universe;
    };
} /* namespace bp */

#endif /* BEEF_PLANET_GAME_OBJECT_H */
