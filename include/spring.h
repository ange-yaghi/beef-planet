#ifndef BEEF_PLANET_SPRING_H
#define BEEF_PLANET_SPRING_H

#include "delta.h"

namespace bp {
    class Spring {
        public:
            Spring();
            ~Spring();

            void process(float dt);
            ysVector calculateF(const ysVector &v, const ysVector &x) const;
            void calculate(
                    const ysVector &v,
                    const ysVector &x,
                    const ysVector &dt,
                    ysVector *a_t,
                    ysVector *v_t);

            ysVector m_ks;
            ysVector m_kd;

            ysVector m_x;
            ysVector m_target;
            ysVector m_v;

            int m_steps;
    };
} /* namespace bp */

#endif /* BEEF_PLANET_SPRING_H */
