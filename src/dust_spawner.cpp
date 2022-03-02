#include "../include/dust_spawner.h"

#include "../include/physical_object.h"
#include "../include/universe.h"

bp::DustSpawner::DustSpawner() {
    m_cooldown = 0.0;
    m_model = nullptr;
    m_spawnRadius = 1000.0f;

    m_maxDustParticles = 1000;
    m_activeDustParticles = 0;
    m_dust = nullptr;
}

bp::DustSpawner::~DustSpawner() {
    delete[] m_dust;
}

void bp::DustSpawner::initialize(
        dbasic::DeltaEngine* engine,
        dbasic::DefaultShaders* shaders,
        Universe* universe)
{
    GameObject::initialize(engine, shaders, universe);

    m_dust = new SpaceDust *[m_maxDustParticles];
}

void bp::DustSpawner::process(float dt) {
    //if (m_cooldown < 0.0) {
        m_cooldown = ysMath::UniformRandom(0.1);

        while (m_activeDustParticles < m_maxDustParticles) {
            createDust();
        }
    //}

    const ysVector playerLocation =
        m_universe->m_player->getPhysicsComponent()->m_transform.GetWorldPosition();

    for (int i = 0; i < m_activeDustParticles; ++i) {
        SpaceDust *dust = m_dust[i];

        const ysVector dustLocation = dust->getPhysicsComponent()->m_transform.GetWorldPosition();
        const float distance = ysMath::GetScalar(
            ysMath::MagnitudeSquared3(ysMath::Sub(playerLocation, dustLocation)));
        const float despawnRadiusScale = 1.2 * m_spawnRadius;
        if (distance > (despawnRadiusScale * despawnRadiusScale)) {
            dust->scheduleDeletion();

            m_dust[i] = m_dust[m_activeDustParticles - 1];
            --i;
            --m_activeDustParticles;
        }
    }

    m_cooldown -= dt;
}

void bp::DustSpawner::render() {
    /* void */
}

void bp::DustSpawner::createDust() {
    SpaceDust* newDust = m_universe->spawn<SpaceDust>();

    const ysVector offset =
        ysMath::LoadVector(
            ysMath::UniformRandom(2 * m_spawnRadius) - m_spawnRadius,
            ysMath::UniformRandom(2 * m_spawnRadius) - m_spawnRadius,
            ysMath::UniformRandom(2 * m_spawnRadius) - m_spawnRadius);

    const ysVector playerLocation =
        m_universe->m_player->getPhysicsComponent()->m_transform.GetWorldPosition();

    newDust->getPhysicsComponent()->m_transform.SetPosition(
            ysMath::Add(playerLocation, offset));
    newDust->getPhysicsComponent()->setVelocity(
            ysMath::LoadVector(
                0.05f * (ysMath::UniformRandom(1.0f) - 0.5f),
                0.05f * (ysMath::UniformRandom(1.0f) - 0.5f),
                0.05f * (ysMath::UniformRandom(1.0f) - 0.5f)));
    newDust->setModel(m_model);
    newDust->updateMass(0.01);
    newDust->getPhysicsComponent()->setEnableGravity(false);

    m_dust[m_activeDustParticles++] = newDust;
}
