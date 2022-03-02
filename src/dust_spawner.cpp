#include "../include/dust_spawner.h"

#include "../include/physical_object.h"
#include "../include/space_dust.h"
#include "../include/universe.h"

bp::DustSpawner::DustSpawner() {
    m_cooldown = 0.0;
    m_model = nullptr;
}

bp::DustSpawner::~DustSpawner() {
    /* void */
}

void bp::DustSpawner::initialize(
        dbasic::DeltaEngine* engine,
        dbasic::DefaultShaders* shaders,
        Universe* universe) {
    GameObject::initialize(engine, shaders, universe);
}

void bp::DustSpawner::process(float dt) {
    if (m_cooldown < 0.0) {
        m_cooldown = ysMath::UniformRandom(0.1);
        createDust();
    }

    m_cooldown -= dt;
}

void bp::DustSpawner::render() {
    /* void */
}

void bp::DustSpawner::createDust() {
    SpaceDust* newDust = m_universe->spawn<SpaceDust>();
    newDust->getPhysicsComponent()->m_transform.SetPosition(
            ysMath::LoadVector(
                ysMath::UniformRandom(1000.0),
                ysMath::UniformRandom(1000.0),
                ysMath::UniformRandom(1000.0)));
    newDust->getPhysicsComponent()->setVelocity(
            ysMath::LoadVector(
                0.05f * (ysMath::UniformRandom(1.0f) - 0.5f),
                0.05f * (ysMath::UniformRandom(1.0f) - 0.5f),
                0.05f * (ysMath::UniformRandom(1.0f) - 0.5f)));
    newDust->setModel(m_model);
    newDust->updateMass(0.01);
    newDust->getPhysicsComponent()->setEnableGravity(false);
}
