#ifndef BEEF_PLANET_SPACE_DUST_H
#define BEEF_PLANET_SPACE_DUST_H

#include "physical_object.h"

#include "physics_component.h"
#include "mouse_controller.h"

#include "delta.h"

namespace bp {
    class SpaceDust : public PhysicalObject {
        public:
            SpaceDust();
            virtual ~SpaceDust();

            virtual void initialize(
                    dbasic::DeltaEngine *engine,
                    dbasic::DefaultShaders *shaders,
                    Universe *universe);

            virtual void process(float dt);
            virtual void render();

            virtual void updateMass(float mass);
    };
} /* namespace bp */

#endif /* BEEF_PLANET_SPACE_DUST_H */
