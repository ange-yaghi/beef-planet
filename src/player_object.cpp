#include "../include/player_object.h"

#include "../include/mouse_controller.h"
#include "../include/universe.h"

bp::PlayerObject::PlayerObject() {
    m_leftGrab = nullptr;
    m_rightGrab = nullptr;

    clearTags();
    addTag(GameObject::TagPlayer);
}

bp::PlayerObject::~PlayerObject() {
    /* void */
}

void bp::PlayerObject::initialize(
        dbasic::DeltaEngine* engine,
        dbasic::DefaultShaders *shaders,
        Universe* universe)
{
    PhysicalObject::initialize(engine, shaders, universe);
    m_mouseController.initialize(engine, shaders, universe);
    updateMass(100.0);
    m_color = ysColor::srgbiToLinear(0xAA, 0xAA, 0xFF);
}

void bp::PlayerObject::process(float dt) {
    PhysicalObject::process(dt);

    if (m_engine->IsKeyDown(ysKey::Code::W)) {
        ysVector movement;
        movement = ysMath::Sub(m_shaders->GetCameraTarget(), m_shaders->GetCameraPosition());
        movement = ysMath::Normalize(movement);
        movement = ysMath::Mul(
                movement,
                ysMath::LoadScalar(20.0 * PhysicalObject::m_physics_component.getMass()));
        getPhysicsComponent()->forceAdd(movement);
    }
    else if (m_engine->IsKeyDown(ysKey::Code::S)) {
        ysVector movement;
        movement = ysMath::Sub(
                m_shaders->GetCameraPosition(),
                m_shaders->GetCameraTarget());
        movement = ysMath::Normalize(movement);
        movement = ysMath::Mul(
                movement,
                ysMath::LoadScalar(20.0 * PhysicalObject::m_physics_component.getMass()));
        getPhysicsComponent()->forceAdd(movement);
    }
    else if (m_engine->IsKeyDown(ysKey::Code::M)) {
        updateMass(1000);
    }

    m_mouseController.process(
            getPhysicsComponent()->m_transform.GetWorldPosition());

    if (m_engine->IsMouseButtonDown(ysMouse::Button::Left)) {
        GameObject *target = m_universe->rayCast(
            ysMath::Normalize(
                ysMath::Sub(m_shaders->GetCameraTarget(), m_shaders->GetCameraPosition())),
            m_shaders->GetCameraPosition(),
            TagPlanet);
        if (target != nullptr) {
            target->setDebugHighlighted(true);
            m_leftGrab = static_cast<PhysicalObject *>(target);
        }
    }
    else if (m_engine->IsMouseButtonDown(ysMouse::Button::Right)) {
        GameObject *target = m_universe->rayCast(
            ysMath::Normalize(
                ysMath::Sub(m_shaders->GetCameraTarget(), m_shaders->GetCameraPosition())),
            m_shaders->GetCameraPosition(),
            TagPlanet);
        if (target != nullptr) {
            target->setDebugHighlighted(true);
            m_rightGrab = static_cast<PhysicalObject *>(target);
        }
    }

    if (m_rightGrab != nullptr && m_rightGrab->isDeleted()) m_rightGrab = nullptr;
    if (m_leftGrab != nullptr && m_leftGrab->isDeleted()) m_leftGrab = nullptr;
}

void bp::PlayerObject::render() {
    m_shaders->ResetBrdfParameters();
    m_shaders->SetBaseColor(m_color);
    m_shaders->SetSpecularRoughness(1.0f);
    m_shaders->SetColorReplace(true);
    m_universe->drawScaleModel(
            m_model,
            m_size,
            nullptr,
            m_physics_component.m_transform.GetWorldTransform());

    ysVector leftHand;
    ysVector rightHand;

    if (m_leftGrab == nullptr) {
        leftHand = ysMath::MatMult(m_mouseController.getOrientation(),
                ysMath::LoadVector(-m_size * 2.0, m_size * 5.0, m_size * 0.5));
        leftHand = ysMath::Add(leftHand, m_physics_component.m_transform.GetWorldPosition());
    }
    else {
        leftHand = m_leftGrab->getPhysicsComponent()->m_transform.GetWorldPosition();
    }
    if (m_rightGrab == nullptr) {
        rightHand = ysMath::MatMult(m_mouseController.getOrientation(),
                ysMath::LoadVector(m_size * 2.0, m_size * 5.0, m_size * 0.5));
        rightHand = ysMath::Add(rightHand, m_physics_component.m_transform.GetWorldPosition());
    }
    else {
        rightHand = m_rightGrab->getPhysicsComponent()->m_transform.GetWorldPosition();
    }

    ysVector leftElbow = ysMath::Add(ysMath::MatMult(m_mouseController.getOrientation(),
            ysMath::LoadVector(-m_size * 2.0, -m_size * 0.25, -m_size * 0.25)),
            m_physics_component.m_transform.GetWorldPosition());
    ysVector rightElbow = ysMath::Add(ysMath::MatMult(m_mouseController.getOrientation(),
            ysMath::LoadVector(m_size * 2.0, -m_size * 0.25, -m_size * 0.25)),
            m_physics_component.m_transform.GetWorldPosition());

    ysVector leftShoulder = ysMath::Add(ysMath::MatMult(m_mouseController.getOrientation(),
            ysMath::LoadVector(-m_size, 0.0, 0.0)),
            m_physics_component.m_transform.GetWorldPosition());
    ysVector rightShoulder = ysMath::Add(ysMath::MatMult(m_mouseController.getOrientation(),
            ysMath::LoadVector(m_size, 0.0, 0.0)),
            m_physics_component.m_transform.GetWorldPosition());

    m_universe->drawScaleModel(m_model, 0.5, nullptr,
            GameObject::lineHelper(leftShoulder, leftElbow));
    m_universe->drawScaleModel(m_model, 0.5, nullptr,
            GameObject::lineHelper(rightShoulder, rightElbow));
    m_universe->drawScaleModel(m_model, 0.5, nullptr,
            GameObject::lineHelper(leftElbow, leftHand));
    m_universe->drawScaleModel(m_model, 0.5, nullptr,
            GameObject::lineHelper(rightElbow, rightHand));
    m_universe->drawScaleModel(m_model, 0.5, nullptr,
            ysMath::TranslationTransform(leftShoulder));
    m_universe->drawScaleModel(m_model, 0.5, nullptr,
            ysMath::TranslationTransform(rightShoulder));
    m_universe->drawScaleModel(m_model, 0.5, nullptr,
            ysMath::TranslationTransform(leftElbow));
    m_universe->drawScaleModel(m_model, 0.5, nullptr,
            ysMath::TranslationTransform(rightElbow));
    m_universe->drawScaleModel(m_model, 1.0, nullptr,
            ysMath::TranslationTransform(leftHand));
    m_universe->drawScaleModel(m_model, 1.0, nullptr,
            ysMath::TranslationTransform(rightHand));

    dbasic::Light glow;
    glow.Active = 1;
    glow.Attenuation0 = 0.0f;
    glow.Attenuation1 = 0.0f;
    glow.Color = ysMath::Mul(
            ysColor::srgbiToLinear(0xAA, 0xAA, 0xFF),
            ysMath::LoadScalar(50.0f));
    glow.Direction = ysVector4(0.0f, 0.0f, 0.0f, 0.0f);
    glow.FalloffEnabled = 1;
    glow.Position = getPhysicsComponent()->m_transform.GetWorldPosition();
    m_universe->addScaleLight(glow);
}

void bp::PlayerObject::updateMass(float mass) {
    PhysicalObject::updateMass(mass);
    m_mouseController.setZoom(m_size * 7);
    m_universe->setScale(1.0 / m_size);
}
