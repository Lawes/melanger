#include <iostream>
#include "state.h"
#include "global.h"
#include "Vector2.h"
#include "shape.h"
#include "Color.h"
#include "sound.h"

using namespace VODZO;

void State::finDeplacement() {
    enDeplacement = false;
    if( !enRotation)
        shape_->setColor(Color::black);
    if( good() )
        Mixer::play("s1");
}

void State::finRotation() {
    enRotation = false;
    rot_ = rot_%4;
    *(shape_->getRotate()) = getRotation();
    std::cout << rot_ << std::endl;
    if( !enDeplacement)
        shape_->setColor(Color::black);
    if( good() )
        Mixer::play("s1");
}

void State::init(int cible, int p, int rotation, const Vector2& pos, int taille) {
    id_ = cible;
    pos_ = p;
    rot_ = rotation;

    enDeplacement = false;
    enRotation = false;

    shape_->setTransform(pos, getRotation() , taille);
    shape_->apply();

}


bool State::good() {
    return ( pos_==id_
        && rot_ == 0
        && !enDeplacement
        && !enRotation);
}

void State::goPos(const int newPos) {
    pos_ = newPos;
    enDeplacement = true;
    shape_->setColor(Color::red);
}

void State::goRotation(SensRotation rot) {
    rot_ += static_cast<int>(rot);
    enRotation = true;
    shape_->setColor(Color::red);
}
