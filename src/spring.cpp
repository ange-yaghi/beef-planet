#include "../include/spring.h"

bp::Spring::Spring() {
    m_ks = m_kd = ysMath::Constants::Zero;
    m_x = m_target = ysMath::Constants::Zero;
    m_v = ysMath::Constants::Zero;

    m_steps = 10;
}

bp::Spring::~Spring() {
    /* void */
}

ysVector bp::Spring::calculateF(const ysVector &v, const ysVector &x) const {
    const ysVector delta = ysMath::Sub(m_target, x);
    ysVector F = ysMath::Mul(
            m_ks,
            delta);

    return ysMath::Add(
        F,
        ysMath::Mul(
            m_kd,
            ysMath::Negate(v)));
}

void bp::Spring::calculate(
        const ysVector &v,
        const ysVector &x,
        const ysVector &dt,
        ysVector *a_t,
        ysVector *v_t)
{
    *v_t = v;
    *a_t = calculateF(v, x);
}

void bp::Spring::process(float dt) {
    const ysVector dt_v = ysMath::LoadScalar(dt / m_steps);
    for (int i = 0; i < m_steps; ++i) {
        ysVector a1, a2, a3, a4;
        ysVector v1, v2, v3, v4;

        calculate(m_v, m_x, ysMath::Constants::Zero, &a1, &v1);
        calculate(
                ysMath::Add(
                    m_v,
                    ysMath::Mul(dt_v, ysMath::Mul(a1, ysMath::Constants::Half))),
                ysMath::Add(
                    m_x,
                    ysMath::Mul(dt_v, ysMath::Mul(v1, ysMath::Constants::Half))),
                ysMath::Mul(dt_v, ysMath::Constants::Half),
                &a2,
                &v2);
        calculate(
                ysMath::Add(
                    m_v,
                    ysMath::Mul(dt_v, ysMath::Mul(a2, ysMath::Constants::Half))),
                ysMath::Add(
                    m_x,
                    ysMath::Mul(dt_v, ysMath::Mul(v2, ysMath::Constants::Half))),
                ysMath::Mul(dt_v, ysMath::Constants::Half),
                &a3,
                &v3);
        calculate(
                ysMath::Add(
                    m_v,
                    ysMath::Mul(dt_v, a3)),
                ysMath::Add(
                    m_x,
                    ysMath::Mul(dt_v, v3)),
                dt_v,
                &a4,
                &v4);

        ysVector dv = a1;
        dv = ysMath::Add(dv, ysMath::Add(a2, a2));
        dv = ysMath::Add(dv, ysMath::Add(a3, a3));
        dv = ysMath::Add(dv, a4);
        dv = ysMath::Mul(dv, dt_v);
        dv = ysMath::Mul(dv, ysMath::LoadScalar(1 / 6.0f));

        ysVector dx = v1;
        dx = ysMath::Add(dx, ysMath::Add(v2, v2));
        dx = ysMath::Add(dx, ysMath::Add(v3, v3));
        dx = ysMath::Add(dx, v4);
        dx = ysMath::Mul(dx, dt_v);
        dx = ysMath::Mul(dx, ysMath::LoadScalar(1 / 6.0f));

        m_v = ysMath::Add(m_v, dv);
        m_x = ysMath::Add(m_x, dx);
    }
}
