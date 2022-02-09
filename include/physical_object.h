#ifndef BEEF_PLANET_PHYSICAL_OBJECT_H
#define BEEF_PLANET_PHYSICAL_OBJECT_H

#include "game_object.h"

#include "physics_component.h"

#include "delta.h"

namespace bp {

    class PhysicalObject : public GameObject {
        public:
            PhysicalObject();
            virtual ~PhysicalObject();

            virtual void initialize(
                    dbasic::DeltaEngine* engine,
                    dbasic::DefaultShaders* shaders,
                    Universe* universe);

            virtual void process(float dt);
            virtual void render();

            void setModel(dbasic::ModelAsset* asset) { m_model = asset; }

            PhysicsComponent *getPhysicsComponent() { return &m_physicsComponent; }

            virtual void updateMass(float mass);
            float getSize() { return m_size; }

            virtual bool rayTest(const ysVector &d, const ysVector &p0, float *s);
            virtual void free();

        protected:
            PhysicsComponent m_physicsComponent;

            float m_size;
            float m_sizeGoal;
            dbasic::ModelAsset* m_model;
            ysVector m_color;
    };

} /* namespace bp */

#endif /* BEEF_PLANET_PHYSICAL_OBJECT_H */
