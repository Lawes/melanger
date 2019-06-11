#ifndef PSM_EMITTER_H_INCLUDED
#define PSM_EMITTER_H_INCLUDED

#include "psm_type.h"

namespace PSM {
	struct Particule;

	class Emitter {
		public:
		Emitter();
		virtual ~Emitter(){};

		virtual void setPos(const PSM::Vector& v) {
		    refPos = v;
		}

		virtual void setVit(const PSM::Vector& v, float s=0.0f) {
            refVit = v;
            stdVit = s;
		}

        void setVie(float v) {
            refVie = v;
        }

        void setStdVie(float s) {
            stdVie = s;
        }

        float getLife() const { return refVie; }

        void setTaille(float t, float s = 0.0f) {
            refTaille = t;
            stdTaille = s;
        }

		void setPhysic(float fric, float vitAn) {
		    refFriction = fric; refVitAngul = vitAn;
		}

		void setColor(const PSM::Color& col, float s = 0.0f) {
            refColor = col;
            stdCol = s;
		}

        bool isPausedQ() const { return paused; }
        void pause() { paused = true; }
        void run() { paused = false; }

		void operator()(Particule& p);

		private:
			virtual void _newParticule(Particule& p) {}

		protected:
            PSM::Vector refPos, refVit;
            float refVie, refFriction, refTaille, refVitAngul;
            float stdVie, stdCol, stdTaille, stdVit;
            PSM::Color refColor;
            bool paused;

	};


	class HoleEmitter : public Emitter {
		private:
			float m_r, m_mg, stdDir;
			void _newParticule(Particule& p);

		public:
            HoleEmitter() : m_r(0.0f), m_mg(1.0f), stdDir(0.0f)  {  }


			void setRayon(float r) {
				m_r = r;
			}
			void setVit(const PSM::Vector& v, float s=0.0f) {
				refVit = v;
				stdDir = s;
				m_mg = length(refVit);
                refVit /= m_mg;
			}
	};


    class LineEmitter : public Emitter {
        public:
            LineEmitter() {}
            void setLine(const PSM::Vector& a, const PSM::Vector& b) {
                m_a = a;
                m_b = b;
            }

        private:
			void _newParticule(Particule& p);
            PSM::Vector m_a, m_b;
    };

    class SphereEmitter : public Emitter {
        public:
            SphereEmitter(): m_rayon(1.0f)   {       }

            void setRayon(float rayon) {
                m_rayon = rayon;
            }

        private:
			void _newParticule(Particule& p);
            float m_rayon;
    };



    class Spray : public Emitter {
		public:

        Spray() : stdDir(0.0f)  {       }

        void setStdDir(float std) {
            stdDir = std;
        }
		private:
			void _newParticule(Particule& p);

		protected:
            float stdDir;

	};

	class Explo : public Emitter {
		public:
			void setVit(const PSM::Vector& v, float s=0.0f) {
				//refVit = v;
				m_mg = length(v);
                //refVit /= m_mg;
			}
		private:
			void _newParticule(Particule& p);

        protected:
            float m_mg;
	};

    class ZoneEmitter : public Emitter {
        public:
            void setZone(float xmin, float ymin, float xmax, float ymax) {
                m_xmin = xmin;
                m_xmax = xmax;
                m_ymin = ymin;
                m_ymax = ymax;
            }
        private:
            void _newParticule(Particule& p);

        protected:
            float m_xmin, m_ymin, m_xmax, m_ymax;
    };

}
#endif // PSM_EMITTER_H_INCLUDED
