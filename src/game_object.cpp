#include "../include/game_object.h"

bp::GameObject::GameObject() {
    m_id = 0;
    m_tags = 0;
    m_deleted = false;
    m_engine = nullptr;
    m_shaders = nullptr;
    m_universe = nullptr;
    m_debugHighlighted = false;
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

bool bp::GameObject::rayTest(const ysVector &d, const ysVector &p0, float *s) {
    return false;
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

ysMatrix bp::GameObject::sphereHelper(const ysMatrix &transform, float radius) {
    return ysMath::MatMult(
        transform,
        ysMath::ScaleTransform(ysMath::LoadScalar(radius)));
}

ysMatrix bp::GameObject::sphereHelper(const ysVector &position, float radius) {
    return ysMath::MatMult(
        ysMath::TranslationTransform(position),
        ysMath::ScaleTransform(ysMath::LoadScalar(radius)));
}

ysMatrix bp::GameObject::lineHelper(ysVector start, ysVector end, float radius)
{
    ysVector distance = ysMath::Mul(ysMath::Sub(end, start), ysMath::Constants::Half);

    ysVector axis1 = ysMath::Normalize(ysMath::Cross(distance, ysMath::Constants::YAxis));
    ysVector axis2 = ysMath::Normalize(ysMath::Cross(distance, axis1));

    ysMatrix radiusScale = ysMath::ScaleTransform(ysMath::LoadVector(1.0, radius, radius, 1.0));
    ysMatrix scaleMatrix = ysMath::Transpose(
            ysMath::LoadMatrix(distance, axis1, axis2, ysMath::Constants::IdentityRow4));
    
    ysMatrix translationMatrix = ysMath::TranslationTransform(
            ysMath::Mul(ysMath::Add(start, end), ysMath::LoadScalar(0.5)));

    return ysMath::MatMult(
        ysMath::MatMult(translationMatrix, scaleMatrix),
        radiusScale);
}
