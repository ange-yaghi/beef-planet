#include "..\include\gravity_simulator.h"
#include "..\include\physical_object.h"

#include <assert.h>

bp::GravitySimulator::GravitySimulator()
{
    m_G = 0.3;
}

bp::GravitySimulator::~GravitySimulator()
{

}

void bp::GravitySimulator::integrate(float dt)
{
    for (PhysicsComponent* component : m_components) {
        component->integrate(dt);
    }
}

void bp::GravitySimulator::clearForces() {
    for (int i = 0; i < m_components.size(); i++) {
        m_components[i]->forceClear();
        m_components[i]->clearIntersections();
    }
}

void bp::GravitySimulator::generateForces()
{
    for (PhysicsComponent* componentA : m_components) {
        assert(componentA->getInverseMass() != 0);
    }

    float inverseG = 1.0 / m_G;
    for (int i = 0; i < m_components.size(); i++) {
        for (int j = i + 1; j < m_components.size(); j++) {
            ysVector inverse_mass_mult = ysMath::LoadScalar(-m_components[i]->getInverseMass() * m_components[j]->getInverseMass() * inverseG);
            ysVector delta = ysMath::Sub(m_components[i]->m_transform.GetWorldPosition(), m_components[j]->m_transform.GetWorldPosition());
            ysVector magnitude = ysMath::Magnitude(delta);
            // = |r| ^ 3 / (m1 * m2)
            ysVector tempMath = ysMath::Mul(magnitude, ysMath::Mul(magnitude, ysMath::Mul(magnitude, inverse_mass_mult)));
            // = r * m1 * m2 / |r| ^ 3
            ysVector force = ysMath::Div(delta, tempMath);

            float  distance = ysMath::GetScalar(magnitude) - (m_components[i]->getParent()->getSize() + m_components[j]->getParent()->getSize());
            if (distance <= 0) { // Elastic collision from objects
                m_components[i]->addIntersection(m_components[j]->getParent());
                m_components[j]->addIntersection(m_components[i]->getParent());
                // = kx = r * d * k / |r|
                ysVector springForce = ysMath::Mul(ysMath::Div(delta, magnitude), ysMath::LoadScalar(distance * 5000.0));
                m_components[i]->forceAdd(ysMath::Negate(springForce));
                m_components[j]->forceAdd(springForce);
            }
            else { // Gravity from interacting objects
                m_components[i]->forceAdd(force);
                m_components[j]->forceAdd(ysMath::Negate(force));
            }
        }
    }
}

void bp::GravitySimulator::registerComponent(PhysicsComponent *component)
{
    m_components.push_back(component);
}

void bp::GravitySimulator::deregisterComponent(PhysicsComponent *component)
{
    int index = -1;
    for (int i = 0; i < m_components.size(); ++i) {
        if (m_components[i] == component) {
            index = i;
            break;
        }
    }
    
    assert(index != -1);

    m_components[index] = m_components.back();
    m_components.resize(m_components.size() - 1);
}
