#include "../include/game_object.h"

bp::GameObject::GameObject() {
    m_id = 0;
    m_deleted = false;
    m_engine = nullptr;
    m_shaders = nullptr;
    m_universe = nullptr;
}

bp::GameObject::~GameObject() {
    /* void */
}

void bp::GameObject::initialize(
        dbasic::DeltaEngine* engine,
        dbasic::DefaultShaders* shaders,
        Universe *universe)
{
    m_engine = engine;
    m_shaders = shaders;
    m_universe = universe;
}

void bp::GameObject::process(float dt) {
    /* void */
}

void bp::GameObject::render() {
    /* void */
}

void bp::GameObject::free() {
    /* void */
}
