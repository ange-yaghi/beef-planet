#include "../include/physical_object.h"

#include "../include/universe.h"

#include <math.h>

bp::PhysicalObject::PhysicalObject() {
    m_model = nullptr;
    m_size = 1.0;
    m_physicsComponent.setParent(this);
    m_color = ysMath::Constants::Zero;
    addTag(GameObject::TagPlanet);

    m_sizeAnimator.m_kd = ysMath::LoadScalar(20.0f);
    m_sizeAnimator.m_ks = ysMath::LoadScalar(1000.0f);
    m_sizeAnimator.m_steps = 1;
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

    universe->getGravitySimulator()->registerComponent(&m_physicsComponent);

    m_color = ysColor::srgbiToLinear(
            ysMath::UniformRandomInt(200) + 55,
            ysMath::UniformRandomInt(200) + 55,
            ysMath::UniformRandomInt(200) + 55);
}

void bp::PhysicalObject::process(float dt) {
    m_sizeAnimator.process(dt);
    m_size = ysMath::GetScalar(m_sizeAnimator.m_x);

    const int collisionCount = m_physicsComponent.getIntersectionCount();
    for (int i = 0; i < collisionCount; ++i) {
        PhysicalObject* object = m_physicsComponent.getIntersection(i);
        PhysicsComponent* component = object->getPhysicsComponent();

        if (m_physicsComponent.getMass() > component->getMass()) {
            ysVector normal = ysMath::Sub(
                    m_physicsComponent.m_transform.GetWorldPosition(),
                    component->m_transform.GetWorldPosition());
            normal = ysMath::Normalize(normal);

            ysVector velocityDirection = ysMath::Normalize(
                    ysMath::Sub(
                            component->getVelocity(),
                            m_physicsComponent.getVelocity()));
            const float normalDotVel =
                ysMath::GetScalar(ysMath::Dot(normal, velocityDirection));

            if (normalDotVel > 0.7) {
                object->scheduleDeletion();
                float newMass = m_physicsComponent.getMass() + component->getMass();
                m_physicsComponent.setVelocity(ysMath::Div(ysMath::Add(
                        ysMath::Mul(
                                ysMath::LoadScalar(m_physicsComponent.getMass()),
                                m_physicsComponent.getVelocity()),
                        ysMath::Mul(
                                ysMath::LoadScalar(component->getMass() * 0.5),
                                component->getVelocity())),
                    ysMath::LoadScalar(newMass)));
                updateMass(newMass);
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

    ysMatrix transform = ysMath::MatMult(
        m_physicsComponent.m_transform.GetWorldTransform(),
        ysMath::ScaleTransform(ysMath::LoadScalar(m_size)));
    m_universe->drawScaleModel(
            m_model,
            nullptr,
            transform);
}

void bp::PhysicalObject::updateMass(float mass) {
    getPhysicsComponent()->setMass(mass);
    m_sizeAnimator.m_target =
        ysMath::LoadScalar(std::pow(m_physicsComponent.getMass(), 1.0 / 3.0));
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
    m_universe->getGravitySimulator()->deregisterComponent(&m_physicsComponent);
}
