#ifndef PSM_MOVER_H_INCLUDED
#define PSM_MOVER_H_INCLUDED

#include "psm_type.h"


namespace PSM {
    struct Particule;

	class Mover {
		public:

		virtual bool updateParticule(Particule& p, float dt);
	};


	class Espace : public Mover {
		public:
			bool updateParticule(Particule& p, float dt);
	};

	class Earth : public Mover {
		public:

			void setGravity(const Vector &g) { gravity = g; }
			void setWind(const Vector& w) { wind = w;}

			bool updateParticule(Particule& p, float dt);
		private:
			Vector gravity, wind;

	};


	class Vortex : public Mover {
		public :
            Vortex(): m_attraction(1.0f) {}

			void setCenter(const Vector& v) {
                center = v;
			}
			void setAttraction(float f) {
                m_attraction = f;
			}

			bool updateParticule(Particule& p, float dt);

		private:
			Vector center;
			float m_attraction;


	};

}
#endif // PSM_MOVER_H_INCLUDED
