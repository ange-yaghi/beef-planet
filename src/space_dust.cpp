#include "../include/space_dust.h"

#include "../include/mouse_controller.h"
#include "../include/universe.h"

bp::SpaceDust::SpaceDust() {
    m_size = 0.01;
}

bp::SpaceDust::~SpaceDust() {
    /* void */
}

void bp::SpaceDust::initialize(
        dbasic::DeltaEngine* engine,
        dbasic::DefaultShaders *shaders,
        Universe* universe)
{
    PhysicalObject::initialize(engine, shaders, universe);
    m_color = ysColor::srgbiToLinear(0xAA, 0xAA, 0xFF);
}

void bp::SpaceDust::process(float dt) {
    PhysicalObject::process(dt);
}

void bp::SpaceDust::render() {
    m_shaders->ResetBrdfParameters();
    m_shaders->SetBaseColor(m_color);
    m_shaders->SetSpecularRoughness(1.0f);
    m_shaders->SetColorReplace(true);
    m_universe->drawScaleModel(
            m_model,
            m_size,
            nullptr,
            m_physics_component.m_transform.GetWorldTransform());
}

void bp::SpaceDust::updateMass(float mass) {
    PhysicalObject::updateMass(mass);
}
