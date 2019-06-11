#ifndef PSM_VECTOR_EXTENTION
#define PSM_VECTOR_EXTENTION
#include "psm_common.h"
#include <SFML/System.hpp>

#include "vector2_sfml_extention.h"

namespace PSM {
    typedef sf::Vector2f Vector;
    Vector randomVector();
    void cachedRotate(Vector& v, float angle);
}

#endif