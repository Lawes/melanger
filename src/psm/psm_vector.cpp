#include "psm_vector.h"
#include "random.h"

void PSM::cachedRotate(PSM::Vector& v, float angle) {
    float cos = TableCS.Cos(angle),
          sin = TableCS.Sin(angle);

    v = PSM::Vector(
    cos * v.x - sin * v.y,
    sin * v.x + cos * v.y);
    
}

PSM::Vector PSM::randomVector() {
    float angle = Random::Percent()*360.0f;
    return PSM::Vector( TableCS.Cos(angle),
                   TableCS.Sin(angle));
}