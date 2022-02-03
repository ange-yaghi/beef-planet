#include "../include/physical_object.h"

#include "../include/universe.h"

#include <math.h>

bp::PhysicalObject::PhysicalObject() {
    m_model = nullptr;
    m_size = 1.0;
    m_physics_component.setParent(this);
    m_color = ysMath::Constants::Zero;
    addTag(GameObject::TagPlanet);
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
                object->scheduleDeletion();
                const float currentMass = m_physics_component.getMass();
                updateMass(currentMass + component->getMass());
            }
        }
    }
}

void bp::PhysicalObject::render() {
    m_shaders->ResetBrdfParameters();
    if (m_debugHighlighted) {
        m_shaders->SetBaseColor(ysMath::Constants::One);
        m_shaders->SetLit(false);
    }
    else {
        m_shaders->SetBaseColor(m_color);
    }

    m_shaders->SetSpecularRoughness(1.0f);
    m_shaders->SetColorReplace(true);
    m_universe->drawScaleModel(
            m_model,
            m_size,
            nullptr,
            m_physics_component.m_transform.GetWorldTransform());
}

void bp::PhysicalObject::updateMass(float mass) {
    getPhysicsComponent()->setMass(mass);
    m_size = std::pow(m_physics_component.getMass(), 1.0 / 3.0);
}

bool bp::PhysicalObject::rayTest(const ysVector &d, const ysVector &p0, float *s) {
    const ysVector p = ysMath::Mul(
        getPhysicsComponent()->m_transform.GetWorldPosition(),
        ysMath::LoadScalar(m_universe->getScale()));
    const float r = m_size * m_universe->getScale();

    const ysVector d_pos = ysMath::Sub(p0, p);
    const ysVector d_dot_dir = ysMath::Dot(d_pos, d);
    const ysVector mag2 = ysMath::MagnitudeSquared3(d_pos);

    const ysVector radius2 = ysMath::LoadScalar(r * r);
    ysVector det = ysMath::Sub(
            ysMath::Mul(d_dot_dir, d_dot_dir),
            ysMath::Sub(mag2, radius2));

    if (ysMath::GetScalar(det) < 0) {
        return false;
    }
    else {
        det = ysMath::Sqrt(det);
        const ysVector t1 = ysMath::Sub(det, d_dot_dir);
        const ysVector t2 = ysMath::Sub(ysMath::Negate(det), d_dot_dir);

        const float t1_s = ysMath::GetScalar(t1);
        const float t2_s = ysMath::GetScalar(t2);

        const bool intersects = t1_s > 0 || t2_s > 0;
        if (intersects) {
            if (t2_s < t1_s || t1_s <= 0) {
                *s = t2_s;
            }
            else {
                *s = t1_s;
            }
        }

        return intersects;
    }
}

void bp::PhysicalObject::free() {
    m_universe->getGravitySimulator()->deregisterComponent(&m_physics_component);
}
