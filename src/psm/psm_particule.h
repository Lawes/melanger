#ifndef PSM_PARTICULE_H_INCLUDED
#define PSM_PARTICULE_H_INCLUDED

#include <SFML/Graphics.hpp>
#include "psm_type.h"

namespace PSM {
	struct Particule {
	    unsigned int id;
		PSM::Vector pos, oldPos;
		PSM::Vector vel;
		PSM::Color color;
		float vie, angle, taille;
		float vitAngul, friction;
	};

}
#endif // PSM_PARTICULE_H_INCLUDED
