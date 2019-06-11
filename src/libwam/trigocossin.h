#ifndef TRIGOCOSSIN_H
#define TRIGOCOSSIN_H

#include <vector>

class TrigoCosSin
{
    public:

        float Sin(const float deg) const {
            return m_sin[goodCadran(deg)];
        }

        float Cos(const float deg) const {
            return m_cos[goodCadran(deg)];
        }

        TrigoCosSin(const int prec);
        virtual ~TrigoCosSin() {};
    protected:
    private:

        int goodCadran(const float deg) const {
            int angle = static_cast<int>(deg*m_prec)%sc_period;
            if( angle < 0) angle += sc_period;
            return angle;
        }

        int sc_period, m_prec;
        std::vector<float> m_cos, m_sin;
};

#endif // TRIGOCOSSIN_H
