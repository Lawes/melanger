#ifndef PSM_COLOR_HEADER
#define PSM_COLOR_HEADER

#include "color.h"
#include <SFML/Graphics.hpp>

namespace PSM {
	typedef wam::Color Color;
}

sf::Color touint(const wam::Color& c);

/*
PSM::Color operator*(float lhs, const PSM::Color& rhs);

PSM::Color operator*(const PSM::Color& lhs, float rhs);

PSM::Color operator/(const PSM::Color& lhs, float rhs);
*/
#endif