#include "../include/planet_spawner.h"

#include "../include/physical_object.h"
#include "../include/space_dust.h"
#include "../include/universe.h"

bp::PlanetSpawner::PlanetSpawner() {
    m_cooldown = 0.0;
    m_model = nullptr;
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
    if (m_cooldown < 0.0) {
        m_cooldown = ysMath::UniformRandom(0.5);
        createPlanet();
    }

    m_cooldown -= dt;
}

void bp::PlanetSpawner::render() {
    /* void */
}

void bp::PlanetSpawner::createPlanet() {
    PhysicalObject* newObject = m_universe->spawn<PhysicalObject>();
    newObject->getPhysicsComponent()->m_transform.SetPosition(
            ysMath::LoadVector(
                ysMath::UniformRandom(100.0),
                ysMath::UniformRandom(100.0),
                ysMath::UniformRandom(100.0)));
    newObject->setModel(m_model);
    newObject->updateMass(ysMath::UniformRandom(100.0) + 1.0);
}