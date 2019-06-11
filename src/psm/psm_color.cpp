#include "psm_color.h"
#include <iostream>

sf::Color touint(const wam::Color& c) {
    return sf::Color(int(c.r*255.0f), int(c.g*255.0f), int(c.b*255.0f), int(c.a*255.0f));
}

/*
PSM::Color operator*(float lhs, const PSM::Color& rhs)		// left scalar multiplication
{
    return PSM::Color((float)lhs * rhs.r, (float)lhs * rhs.g, (float)lhs * rhs.b, (float)lhs * rhs.a);
}

PSM::Color operator*(const PSM::Color& lhs, float rhs)		// right scalar multiplication
{
    return PSM::Color((float)rhs * lhs.r, (float)rhs * lhs.g, (float)rhs * lhs.b, (float)rhs * lhs.a);
}

PSM::Color operator/(const PSM::Color& lhs, float rhs)
{
    return PSM::Color((float)lhs.r / rhs, (float)lhs.g / rhs, (float)lhs.b / rhs, (float)lhs.a / rhs);
}*/