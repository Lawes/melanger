#ifndef PSM_PATH_COURBES_HEADER
#define PSM_PATH_COURBES_HEADER

#include "psm_type.h"
#include "trigocossin.h"


namespace {
	TrigoCosSin tableCosSin(2);
}

namespace PSM {
    namespace Path2d {

        class Epitrochoid {
            private:
                float m_rayon, m_q, m_k;


            public:
                Epitrochoid() : m_rayon(1.0), m_q(1.0), m_k(1.0) {}

                void init(float r, float p1, float p2) {
                    m_rayon = r/(1.0+m_q);
                    m_q = p1;
                    m_k = p2;
                }

                PSM::Vector operator() (float r) const {
                    float angle = r * 360.0f*(m_q+1.0);
                    float cqt = TableCS.Cos(angle*(m_q+1.0));
                    float sqt = TableCS.Sin(angle*(m_q+1.0));
                    float ct = TableCS.Cos(angle);
                    float st = TableCS.Sin(angle);

                    return PSM::Vector(m_rayon*((m_q+1.0)*ct - m_k*cqt ),
                                       m_rayon*((m_q+1.0)*st - m_k*sqt ));

                }


        };




    }

    /*
namespace Path3d {


class Satellite {
	private:
		float m_rayon, m_alpha, m_k, m_ca, m_sa;

        void setAlpha(float a) {
            m_alpha = a;
            m_ca = TableCS.Cos(a);
            m_sa = TableCS.Sin(a);
        }

	public:

		Satellite() : m_rayon(1.0f), m_alpha(0.0f), m_k(1.0f) {
		    setAlpha(m_alpha);
		}

		void init(float p1, float p2, float p3) {
		    m_rayon = p1;
		    setAlpha(p2);
			m_k = p3;
		}

		PSM::Vector operator() (float r) const {
		    float angle = r * 360.0f;
		    float ct = TableCS.Cos(angle);
		    float st = TableCS.Sin(angle);
		    float ckt = TableCS.Cos(angle*m_k);
		    float skt = TableCS.Sin(angle*m_k);
            return PSM::Vector(m_rayon*(m_ca*ct*ckt-st*skt),
                               m_rayon*(m_ca*st*ckt+ct*skt),
                               m_rayon*m_sa*ckt);

		}
};
*/



}
#endif
