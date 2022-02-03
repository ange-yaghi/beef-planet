#ifndef BEEF_PLANET_PHYSICS_COMPONENT_H
#define BEEF_PLANET_PHYSICS_COMPONENT_H

#include "delta.h"

#include <vector>

namespace bp {
    class PhysicalObject;
    class PhysicsComponent {
        public:
            PhysicsComponent();
            ~PhysicsComponent();

            void integrate(float dt);

            ysTransform m_transform;
            void setMass(float mass);
            void setInverseMass(float inverseMass) { m_inverseMass = inverseMass; }
            float getInverseMass() { return m_inverseMass; }
            float getMass() { return 1.0 / m_inverseMass; }
            bool isMassless() const { return m_massless; }

            void setEnableGravity(bool enable) { m_enableGravity = enable; }
            bool isGravityEnabled() const { return m_enableGravity; }

            void forceClear() { m_forceAccumulator = ysMath::Constants::Zero; }
            void forceAdd(const ysVector &force);

            ysVector getVelocity() { return m_velocity; }
            void setVelocity(const ysVector &velocity) { m_velocity = velocity; }

            void setParent(PhysicalObject* parent) { m_parent = parent; }
            PhysicalObject *getParent() { return m_parent; }

            void clearIntersections();
            void addIntersection(PhysicalObject* secondObject);
            int getIntersectionCount() const { return m_intersections.size(); }
            PhysicalObject *getIntersection(int index) { return m_intersections[index]; }

        private:
            float m_inverseMass;
            bool m_massless;
            bool m_enableGravity;

            ysVector m_forceAccumulator;
            ysVector m_velocity;
            std::vector <PhysicalObject*> m_intersections;
            PhysicalObject* m_parent;
    };
} /* namespace bp */

#endif /* BEEF_PLANET_PHYSICS_COMPONENT_H */
