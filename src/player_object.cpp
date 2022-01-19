#include "..\include\player_object.h"
#include "..\include\mouse_controller.h"
#include "..\include\universe.h"

bp::PlayerObject::PlayerObject()
{
}

bp::PlayerObject::~PlayerObject()
{
}

void bp::PlayerObject::initialize(dbasic::DeltaEngine* engine, dbasic::DefaultShaders *shaders, Universe* universe)
{
    PhysicalObject::initialize(engine, shaders, universe);
    m_mouse_controller.initialize(engine, shaders, universe);
    updateMass(100.0);
    m_color = ysColor::srgbiToLinear(0xAA, 0xAA, 0xFF);
}

void bp::PlayerObject::process(float dt)
{
    PhysicalObject::process(dt);

    if (m_engine->IsKeyDown(ysKey::Code::W)) {
        ysVector movement;
        movement = ysMath::Sub(m_shaders->GetCameraTarget(), m_shaders->GetCameraPosition());
        movement = ysMath::Normalize(movement);
        movement = ysMath::Mul(movement, ysMath::LoadScalar(20.0 * PhysicalObject::m_physics_component.getMass()));
        getPhysicsComponent()->forceAdd(movement);
    }
    else if (m_engine->IsKeyDown(ysKey::Code::S)) {
        ysVector movement;
        movement = ysMath::Sub(m_shaders->GetCameraPosition(), m_shaders->GetCameraTarget());
        movement = ysMath::Normalize(movement);
        movement = ysMath::Mul(movement, ysMath::LoadScalar(20.0 * PhysicalObject::m_physics_component.getMass()));
        getPhysicsComponent()->forceAdd(movement);
    }

    m_mouse_controller.process(getPhysicsComponent()->m_transform.GetWorldPosition());
}

void bp::PlayerObject::render()
{
    m_shaders->ResetBrdfParameters();
    m_shaders->SetBaseColor(m_color);
    m_shaders->SetSpecularRoughness(1.0f);
    //m_engine->SetEmission(ysMath::Mul(ysColor::srgbiToLinear(0xAA, 0xAA, 0xFF), ysMath::LoadScalar(0.2f)));
    m_universe->DrawScaleModel(m_model, m_size, nullptr, m_physics_component.m_transform.GetWorldTransform());

    dbasic::Light glow;
    glow.Active = 1;
    glow.Attenuation0 = 0.0f;
    glow.Attenuation1 = 0.0f;
    glow.Color = ysMath::GetVector4(ysMath::Mul(ysColor::srgbiToLinear(0xAA, 0xAA, 0xFF), ysMath::LoadScalar(50.0f)));
    glow.Direction = ysVector4(0.0f, 0.0f, 0.0f, 0.0f);
    glow.FalloffEnabled = 1;
    glow.Position = ysMath::GetVector4(getPhysicsComponent()->m_transform.GetWorldPosition());
    m_universe->addScaleLight(glow);
}

void bp::PlayerObject::updateMass(float mass)
{
    PhysicalObject::updateMass(mass);
    m_mouse_controller.setZoom(m_size * 7);
    m_universe->setScale(1.0 / m_size);
}
