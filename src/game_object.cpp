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

ysMatrix bp::GameObject::lineHelper(ysVector start, ysVector end)
{
    ysVector distance = ysMath::Sub(end, start);

    ysVector axis1 = ysMath::Normalize(ysMath::Cross(distance, ysMath::Constants::YAxis));
    ysVector axis2 = ysMath::Normalize(ysMath::Cross(distance, axis1));


    ysMatrix scaleMatrix = ysMath::Transpose(
        ysMath::LoadMatrix(
            distance, axis1, axis2, ysMath::Constants::IdentityRow4));
    ysMatrix translationMatrix = ysMath::TranslationTransform(
        ysMath::Mul(
            ysMath::Add(start, end),
            ysMath::LoadScalar(0.5)));
    return ysMath::MatMult(translationMatrix, scaleMatrix);
}


