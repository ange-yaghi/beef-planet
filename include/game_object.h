#ifndef BEEF_PLANET_GAME_OBJECT_H
#define BEEF_PLANET_GAME_OBJECT_H

#include "delta.h"

namespace bp {
    class Universe;
    class GameObject {
        public:
            static const uint64_t TagPlayer = 0x1;
            static const uint64_t TagPlanet = 0x2;

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

            virtual bool rayTest(const ysVector &d, const ysVector &p0, float *s);

            virtual void free();

            static ysMatrix sphereHelper(const ysMatrix &transform, float radius);
            static ysMatrix sphereHelper(const ysVector &position, float radius);
            static ysMatrix lineHelper(ysVector start, ysVector end, float radius);

            void clearTags() { m_tags = 0; }
            void addTag(uint64_t tag) { m_tags |= tag; }
            bool hasTag(uint64_t tag) { return (m_tags & tag) > 0; }

            void setDebugHighlighted(bool highlighted) { m_debugHighlighted = highlighted; }

        private:
            unsigned int m_id;
            bool m_deleted;

            uint64_t m_tags;

        protected:
            dbasic::DeltaEngine* m_engine;
            dbasic::DefaultShaders* m_shaders;
            Universe* m_universe;

        protected:
            bool m_debugHighlighted;
    };
} /* namespace bp */

#endif /* BEEF_PLANET_GAME_OBJECT_H */
