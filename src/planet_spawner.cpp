#include "../include/planet_spawner.h"

#include "../include/physical_object.h"
#include "../include/space_dust.h"
#include "../include/universe.h"

bp::PlanetSpawner::PlanetSpawner() {
    m_cooldown = 0.0f;
    m_model = nullptr;
    m_spawnRadius = 1000.0f;
}

bp::PlanetSpawner::~PlanetSpawner() {
    /* void */
}

void bp::PlanetSpawner::initialize(
        dbasic::DeltaEngine* engine,
        dbasic::DefaultShaders* shaders,
        Universe* universe) {
    GameObject::initialize(engine, shaders, universe);
}

void bp::PlanetSpawner::process(float dt) {
    if (m_cooldown < 0.0f) {
        m_cooldown = ysMath::UniformRandom(0.5f);
        createPlanet();
    }

    m_cooldown -= dt;
}

void bp::PlanetSpawner::render() {
    /* void */
}

void bp::PlanetSpawner::createPlanet() {
    PhysicalObject *newObject = m_universe->spawn<PhysicalObject>();

    const ysVector offset =
        ysMath::LoadVector(
            ysMath::UniformRandom(2 * m_spawnRadius) - m_spawnRadius,
            ysMath::UniformRandom(2 * m_spawnRadius) - m_spawnRadius,
            ysMath::UniformRandom(2 * m_spawnRadius) - m_spawnRadius);

    const ysVector playerLocation =
        m_universe->m_player->getPhysicsComponent()->m_transform.GetWorldPosition();

    newObject->getPhysicsComponent()->m_transform.SetPosition(
        ysMath::Add(playerLocation, offset));
    newObject->getPhysicsComponent()->setVelocity(
        ysMath::LoadVector(
            5.5f * (ysMath::UniformRandom(1.0f) - 0.5f),
            5.5f * (ysMath::UniformRandom(1.0f) - 0.5f),
            5.5f * (ysMath::UniformRandom(1.0f) - 0.5f)));
    newObject->setModel(m_model);
    newObject->updateMass(ysMath::UniformRandom(5000.0) + 1.0);
}

void bp::PlanetSpawner::createPlanetPointSource() {
    PhysicalObject *newObject = m_universe->spawn<PhysicalObject>();
    newObject->getPhysicsComponent()->m_transform.SetPosition(
        ysMath::LoadVector(
            ysMath::UniformRandom(100.0),
            ysMath::UniformRandom(100.0),
            ysMath::UniformRandom(100.0)));
    newObject->setModel(m_model);
    newObject->updateMass(ysMath::UniformRandom(100.0) + 1.0);
}