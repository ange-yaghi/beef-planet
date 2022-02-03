#include "../include/physics_component.h"

bp::PhysicsComponent::PhysicsComponent() {
    m_velocity = ysMath::Constants::Zero;
    m_inverseMass = 1.0;
    m_massless = false;
    m_enableGravity = true;
    m_parent = nullptr;
    m_forceAccumulator = ysMath::Constants::Zero;
}

bp::PhysicsComponent::~PhysicsComponent() {
    /* void */
}

void bp::PhysicsComponent::integrate(float dt) {
    ysVector acceleration;
    ysVector inverse_mass = ysMath::LoadScalar(m_inverseMass);
    ysVector time_step = ysMath::LoadScalar(dt);
    ysVector position = m_transform.GetPositionParentSpace();
    ysVector limiter = ysMath::LoadVector(1000.0, 1000.0, 1000.0, 1000.0);

    acceleration = ysMath::ComponentMax(
            ysMath::ComponentMin(
                ysMath::Mul(m_forceAccumulator, inverse_mass), limiter),
            ysMath::Negate(limiter));
    m_velocity = ysMath::Add(m_velocity, ysMath::Mul(acceleration, time_step));
    m_transform.SetPosition(ysMath::Add(position, ysMath::Mul(m_velocity, time_step)));
}

void bp::PhysicsComponent::setMass(float mass) {
    if (mass == 0) {
        m_inverseMass = 1.0;
        m_massless = true;
    }
    else {
        m_inverseMass = 1.0 / mass;
        m_massless = false;
    }
}

void bp::PhysicsComponent::forceAdd(const ysVector &force) {
    m_forceAccumulator = ysMath::Add(m_forceAccumulator, force);
}

void bp::PhysicsComponent::clearIntersections() {
    m_intersections.clear();
}

void bp::PhysicsComponent::addIntersection(PhysicalObject* secondObject) {
    m_intersections.push_back(secondObject);
}
