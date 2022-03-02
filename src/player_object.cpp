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
    m_sizeAnimator.m_x = ysMath::LoadScalar(5);
    m_color = ysColor::srgbiToLinear(0xAA, 0xAA, 0xFF);
}

void bp::PlayerObject::process(float dt) {
    PhysicalObject::process(dt);
    m_mouseController.setZoom(m_size * 7);
    m_universe->setScale(1.0 / m_size);

    if (m_engine->IsKeyDown(ysKey::Code::W)) {
        ysVector movement;
        movement = ysMath::Sub(m_shaders->GetCameraTarget(), m_shaders->GetCameraPosition());
        movement = ysMath::Normalize(movement);
        movement = ysMath::Mul(
                movement,
                ysMath::LoadScalar(20.0 * PhysicalObject::m_physicsComponent.getMass()));
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
                ysMath::LoadScalar(20.0 * PhysicalObject::m_physicsComponent.getMass()));
        getPhysicsComponent()->forceAdd(movement);
    }
    else if (m_engine->IsKeyDown(ysKey::Code::M)) {
        updateMass(getPhysicsComponent()->getMass() * 1.05);
    }
    else if (m_engine->IsKeyDown(ysKey::Code::N)) {
        updateMass(getPhysicsComponent()->getMass() * .95);

    }

    m_mouseController.process(
            getPhysicsComponent()->m_transform.GetWorldPosition());

    if (m_engine->ProcessMouseButtonDown(ysMouse::Button::Left)) {
        GameObject *target = m_universe->rayCast(
            ysMath::Normalize(
                ysMath::Sub(m_shaders->GetCameraTarget(), m_shaders->GetCameraPosition())),
            m_shaders->GetCameraPosition(),
            TagPlanet);
        if (target != nullptr) {
            //target->setDebugHighlighted(true);
            m_leftGrab = static_cast<PhysicalObject *>(target);
        }
    }
    else if (m_engine->ProcessMouseButtonUp(ysMouse::Button::Left)) {
        m_leftGrab = nullptr;
    }

    if (m_engine->ProcessMouseButtonDown(ysMouse::Button::Right)) {
        GameObject *target = m_universe->rayCast(
            ysMath::Normalize(
                ysMath::Sub(m_shaders->GetCameraTarget(), m_shaders->GetCameraPosition())),
            m_shaders->GetCameraPosition(),
            TagPlanet);
        if (target != nullptr) {
            //target->setDebugHighlighted(true);
            m_rightGrab = static_cast<PhysicalObject *>(target);
        }
    }
    else if (m_engine->ProcessMouseButtonUp(ysMouse::Button::Right)) {
        m_rightGrab = nullptr;
    }

    if (m_leftGrab != nullptr && m_leftGrab->isDeleted()) m_leftGrab = nullptr;
    if (m_rightGrab != nullptr && m_rightGrab->isDeleted()) m_rightGrab = nullptr;

    if (m_leftGrab != nullptr) {
        ysVector movement;
        m_leftGrab->getPhysicsComponent()->m_transform.GetWorldPosition();
        movement = ysMath::Sub(
            getPhysicsComponent()->m_transform.GetWorldPosition(),
            m_leftGrab->getPhysicsComponent()->m_transform.GetWorldPosition());
        movement = ysMath::Normalize(movement);
        movement = ysMath::Mul(
            movement,
            ysMath::LoadScalar(20.0 * PhysicalObject::m_physicsComponent.getMass()));
        m_leftGrab->getPhysicsComponent()->forceAdd(movement);
        getPhysicsComponent()->forceAdd(ysMath::Negate(movement));
    }
    if (m_rightGrab != nullptr) {
        ysVector movement;
        m_rightGrab->getPhysicsComponent()->m_transform.GetWorldPosition();
        movement = ysMath::Sub(
            getPhysicsComponent()->m_transform.GetWorldPosition(),
            m_rightGrab->getPhysicsComponent()->m_transform.GetWorldPosition());
        movement = ysMath::Normalize(movement);
        movement = ysMath::Mul(
            movement,
            ysMath::LoadScalar(20.0 * PhysicalObject::m_physicsComponent.getMass()));
        m_rightGrab->getPhysicsComponent()->forceAdd(movement);
        getPhysicsComponent()->forceAdd(ysMath::Negate(movement));
    }
}

void bp::PlayerObject::render() {
    m_shaders->ResetBrdfParameters();
    m_shaders->SetBaseColor(m_color);
    m_shaders->SetSpecularRoughness(1.0f);
    m_shaders->SetColorReplace(true);;
    
    m_universe->drawScaleModel(
            m_model,
            nullptr,
            sphereHelper(m_physicsComponent.m_transform.GetWorldTransform(), m_size));

    ysVector leftHand, rightHand, leftElbow, rightElbow;

    const ysVector leftShoulder = ysMath::Add(ysMath::MatMult(m_mouseController.getOrientation(),
        ysMath::LoadVector(-m_size, 0.0, 0.0)),
        m_physicsComponent.m_transform.GetWorldPosition());
    const ysVector rightShoulder = ysMath::Add(ysMath::MatMult(m_mouseController.getOrientation(),
        ysMath::LoadVector(m_size, 0.0, 0.0)),
        m_physicsComponent.m_transform.GetWorldPosition());

    double elbowRadius = m_size;

    if (m_leftGrab == nullptr) {
        leftHand = ysMath::MatMult(m_mouseController.getOrientation(),
                ysMath::LoadVector(-m_size * 2.0, m_size * 5.0, m_size * 0.5));
        leftHand = ysMath::Add(leftHand, m_physicsComponent.m_transform.GetWorldPosition());
        leftElbow = ysMath::Add(ysMath::MatMult(m_mouseController.getOrientation(),
            ysMath::LoadVector(-m_size * 2.0, -m_size * 0.25, -m_size * 0.25)),
            m_physicsComponent.m_transform.GetWorldPosition());

    }
    else {
        const ysVector planetPosition = m_leftGrab->getPhysicsComponent()->m_transform.GetWorldPosition();
        const ysVector delta = ysMath::Normalize(ysMath::Sub(leftShoulder, planetPosition));
        const ysVector radius = ysMath::LoadScalar(m_leftGrab->getSize());

        leftHand = ysMath::Add(planetPosition, ysMath::Mul(delta, radius));

        ysVector leftDelta = ysMath::Sub(leftHand, leftShoulder);
        ysVector leftProjection = ysMath::MatMult(
            ysMath::Transpose(m_mouseController.getOrientation()), leftDelta);
        if (ysMath::GetX(leftProjection) < 0) {
            leftElbow = ysMath::Add(
                ysMath::Mul(ysMath::Normalize(leftDelta), ysMath::LoadScalar(elbowRadius)),
                leftShoulder);
        }
        else {
            leftElbow = ysMath::Add(ysMath::Mul(
                ysMath::Normalize(ysMath::MatMult(
                    m_mouseController.getOrientation(),
                    ysMath::Mask(leftProjection, ysMath::Constants::MaskOffX))),
                ysMath::LoadScalar(elbowRadius)), leftShoulder);
        }
    }

    if (m_rightGrab == nullptr) {
        rightHand = ysMath::MatMult(m_mouseController.getOrientation(),
                ysMath::LoadVector(m_size * 2.0, m_size * 5.0, m_size * 0.5));
        rightHand = ysMath::Add(rightHand, m_physicsComponent.m_transform.GetWorldPosition());
        rightElbow = ysMath::Add(ysMath::MatMult(m_mouseController.getOrientation(),
            ysMath::LoadVector(m_size * 2.0, -m_size * 0.25, -m_size * 0.25)),
            m_physicsComponent.m_transform.GetWorldPosition());
    }
    else {
        const ysVector planetPosition = m_rightGrab->getPhysicsComponent()->m_transform.GetWorldPosition();
        const ysVector delta = ysMath::Normalize(ysMath::Sub(rightShoulder, planetPosition));
        const ysVector radius = ysMath::LoadScalar(m_rightGrab->getSize());

        rightHand = ysMath::Add(planetPosition, ysMath::Mul(delta, radius));

        ysVector rightDelta = ysMath::Sub(rightHand, rightShoulder);
        ysVector rightProjection = ysMath::MatMult(
                ysMath::Transpose(m_mouseController.getOrientation()), rightDelta);
        if (ysMath::GetX(rightProjection) > 0) {
            rightElbow = ysMath::Add(
                    ysMath::Mul(ysMath::Normalize(rightDelta), ysMath::LoadScalar(elbowRadius)),
                    rightShoulder);
        }
        else {
            rightElbow = ysMath::Add(ysMath::Mul(
                        ysMath::Normalize(ysMath::MatMult(
                                m_mouseController.getOrientation(), 
                                ysMath::Mask(rightProjection, ysMath::Constants::MaskOffX))),
                        ysMath::LoadScalar(elbowRadius)), rightShoulder);
        }
    }

    m_universe->drawScaleModel(m_model, nullptr,
            GameObject::lineHelper(leftShoulder, leftElbow, 0.25 * m_size));
    m_universe->drawScaleModel(m_model, nullptr,
            GameObject::lineHelper(rightShoulder, rightElbow, 0.25 * m_size));
    m_universe->drawScaleModel(m_model, nullptr,
            GameObject::lineHelper(leftElbow, leftHand, 0.25 * m_size));
    m_universe->drawScaleModel(m_model, nullptr,
            GameObject::lineHelper(rightElbow, rightHand, 0.25 * m_size));

    m_universe->drawScaleModel(m_model, nullptr,
            sphereHelper(leftShoulder, 0.25 * m_size));
    m_universe->drawScaleModel(m_model, nullptr,
            sphereHelper(rightShoulder, 0.25 * m_size));
    m_universe->drawScaleModel(m_model, nullptr,
            sphereHelper(leftElbow, 0.2 * m_size));
    m_universe->drawScaleModel(m_model, nullptr,
            sphereHelper(rightElbow, 0.2 * m_size));
    m_universe->drawScaleModel(m_model, nullptr,
            sphereHelper(leftHand, 0.4 * m_size));
    m_universe->drawScaleModel(m_model, nullptr,
            sphereHelper(rightHand, 0.4 * m_size));

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