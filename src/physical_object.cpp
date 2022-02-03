#include "../include/physical_object.h"

#include "../include/universe.h"

#include <math.h>

bp::PhysicalObject::PhysicalObject() {
    m_model = nullptr;
    m_size = 1.0;
    m_physics_component.setParent(this);
    m_color = ysMath::Constants::Zero;
}

bp::PhysicalObject::~PhysicalObject() {
    /* void */
}

void bp::PhysicalObject::initialize(
        dbasic::DeltaEngine* engine,
        dbasic::DefaultShaders* shaders,
        Universe* universe)
{
    GameObject::initialize(engine, shaders, universe);

    universe->getGravitySimulator()->registerComponent(&m_physics_component);

    m_color = ysColor::srgbiToLinear(
            ysMath::UniformRandomInt(200) + 55,
            ysMath::UniformRandomInt(200) + 55,
            ysMath::UniformRandomInt(200) + 55);
}

void bp::PhysicalObject::process(float dt) {
    const int collisionCount = m_physics_component.getIntersectionCount();
    for (int i = 0; i < collisionCount; ++i) {
        PhysicalObject* object = m_physics_component.getIntersection(i);
        PhysicsComponent* component = object->getPhysicsComponent();

        if (m_physics_component.getMass() > component->getMass()) {
            ysVector normal = ysMath::Sub(
                    m_physics_component.m_transform.GetWorldPosition(),
                    component->m_transform.GetWorldPosition());
            normal = ysMath::Normalize(normal);

            ysVector velocityDirection = ysMath::Normalize(
                    ysMath::Sub(
                        component->getVelocity(),
                        m_physics_component.getVelocity()));
            const float normalDotVel =
                ysMath::GetScalar(ysMath::Dot(normal, velocityDirection));

            if (normalDotVel > 0.5) {
                object->destroyMe();
                const float currentMass = m_physics_component.getMass();
                updateMass(currentMass + component->getMass());
            }
        }
    }
}

void bp::PhysicalObject::render() {
    m_shaders->ResetBrdfParameters();
    m_shaders->SetBaseColor(m_color);
    m_shaders->SetSpecularRoughness(1.0f);
    m_shaders->SetColorReplace(true);
    m_universe->DrawScaleModel(
            m_model,
            m_size,
            nullptr,
            m_physics_component.m_transform.GetWorldTransform());
}

void bp::PhysicalObject::updateMass(float mass) {
    getPhysicsComponent()->setMass(mass);
    m_size = std::pow(m_physics_component.getMass(), 1.0 / 3.0);
}

void bp::PhysicalObject::free() {
    m_universe->getGravitySimulator()->deregisterComponent(&m_physics_component);
}
