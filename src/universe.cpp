#include "../include/universe.h"

bp::Universe::Universe() {
    m_engine = nullptr;
    m_shaders = nullptr;
    m_player = nullptr;
    m_timeScale = 1.0;
}

bp::Universe::~Universe() {
    /* void */
}

void bp::Universe::initialize(
        dbasic::DeltaEngine* engine,
        dbasic::DefaultShaders* shaders)
{
    m_engine = engine;
    m_shaders = shaders;
}

void bp::Universe::process(float dt) {
    m_gravity_simulator.integrate(dt);
    worldDestroyer();
    processSpawnQueue();

    m_gravity_simulator.clearForces();
    m_gravity_simulator.generateForces();

    for (GameObject* object : m_game_objects) {
        object->process(dt);
    }
}

void bp::Universe::render() {
    for (GameObject* object : m_game_objects) {
        object->render();
    }
}

void bp::Universe::drawScaleModel(
        dbasic::ModelAsset *model,
        ysTexture *texture,
        const ysMatrix &transform)
{
    ysMatrix scaledTransform = transform;

    scaledTransform = ysMath::Transpose(scaledTransform);
    scaledTransform.rows[3] = ysMath::Mul(
            scaledTransform.rows[3],
            ysMath::LoadVector(m_scale, m_scale, m_scale, 1.0));
    scaledTransform = ysMath::Transpose(scaledTransform);
    scaledTransform = ysMath::MatMult(
        scaledTransform,
        ysMath::ScaleTransform(ysMath::LoadScalar(m_scale)));
    m_shaders->SetObjectTransform(scaledTransform);
    m_engine->DrawModel(m_shaders->GetRegularFlags(), model);
}

void bp::Universe::addScaleLight(dbasic::Light& light) {
    light.Position.Scale(m_scale);
    m_shaders->AddLight(light);
}

bp::GameObject *bp::Universe::rayCast(const ysVector &d, const ysVector &p0, uint64_t tags) {
    float min_s = FLT_MAX;
    GameObject *closest = nullptr;
    for (GameObject *object : m_game_objects) {
        if (!object->hasTag(tags)) {
            continue;
        }

        float s;
        const bool intersects = object->rayTest(d, p0, &s);
        if (intersects && s < min_s) {
            closest = object;
            min_s = s;
        }
    }

    return closest;
}

void bp::Universe::processSpawnQueue() {
    for (GameObject* current_spawn : m_spawn_queue) {
        m_game_objects.push_back(current_spawn);
    }
    m_spawn_queue.clear();
}

void bp::Universe::worldDestroyer() {
    const int gameObjectCount = m_game_objects.size();
    int j = 0;
    for (int i = 0; i < gameObjectCount; ++i) {
        m_game_objects[j] = m_game_objects[i];
        if (!m_game_objects[i]->isDeleted()) {
            ++j;
        }
        else if (!m_game_objects[i]->hasTag(GameObject::TagPlayer)) {
            m_game_objects[i]->free();
            delete m_game_objects[i];
        }
    }
    m_game_objects.resize(j);
}
