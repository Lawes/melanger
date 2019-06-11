#include "trigocossin.h"
#include <cmath>

const float pi180 = 3.14159265/180.0;

TrigoCosSin::TrigoCosSin(const int prec)
            : sc_period(360*prec),
              m_prec(prec)
{

    m_cos.resize(sc_period);
    m_sin.resize(sc_period);
    for(int angle = 0; angle <sc_period; ++angle) {
        float r = static_cast<float>(angle)/prec;
        m_cos[angle] = cos(r*pi180);
        m_sin[angle] = sin(r*pi180);
    }
}
