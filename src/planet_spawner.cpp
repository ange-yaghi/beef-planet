#include "..\include\planet_spawner.h"
#include "..\include\physical_object.h"
#include "..\include\universe.h"

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
        Universe* universe)
{
    GameObject::initialize(engine, shaders, universe);
}

void bp::PlanetSpawner::process(float dt) {
    if (m_cooldown < 0.0) {
        m_cooldown = ysMath::UniformRandom(0.5);
        planetCreation();
    }

    m_cooldown -= dt;
}

void bp::PlanetSpawner::render() {
    /* void */
}

void bp::PlanetSpawner::planetCreation() {
    PhysicalObject* newObject1 = m_universe->spawn<PhysicalObject>();
    newObject1->getPhysicsComponent()->m_transform.SetPosition(
            ysMath::LoadVector(
                ysMath::UniformRandom(100.0),
                ysMath::UniformRandom(100.0),
                ysMath::UniformRandom(100.0)));
    newObject1->setModel(m_model);
    newObject1->updateMass(ysMath::UniformRandom(100.0) + 1.0);
}
