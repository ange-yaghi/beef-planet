#ifndef BEEF_PLANET_PLAYER_OBJECT_H
#define BEEF_PLANET_PLAYER_OBJECT_H

#include "physical_object.h"

#include "physics_component.h"
#include "mouse_controller.h"

#include "delta.h"

namespace bp {
    class PlayerObject : public PhysicalObject {
    public:
        PlayerObject();
        virtual ~PlayerObject();

        virtual void initialize(
            dbasic::DeltaEngine* engine,
            dbasic::DefaultShaders* shaders,
            Universe* universe);

        virtual void process(float dt);
        virtual void render();

    private:
        MouseController m_mouseController;
        PhysicalObject *m_leftGrab;
        PhysicalObject *m_rightGrab;
    };
} /* namespace bp */

#endif /* BEEF_PLANET_PLAYER_OBJECT_H */
