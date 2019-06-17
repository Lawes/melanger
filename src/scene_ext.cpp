
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp> 

#include "intervals.h"
#include "psm.h"


#include <iostream>

#include "scene_ext.h"
#include "globals.h"

using namespace std;


Exemple::Exemple(SceneSwitcher *parent) : 
    Scene(parent)
{ }

void Exemple::load() {
    sf::Image *img;
    RM.get("test", img);
    RM.get("cadre", cadre);

    bg.loadFromImage(*img);
    

    shape.setTexture(&bg, cadre);

    shape.init(0,0,0, sf::Vector2f(200,200), 200);

    auto s = bg.getSize();

    shape.setTextcoords(sf::FloatRect(0,0, s.x/3, s.y/3 ));

    m_actions.To(0, shape.getTranslate(), sf::Vector2f(400,400), 10);
    m_actions.To(1, shape.getRotate(), 360.0f, 10);

}

void Exemple::_begin() {
    m_actions.start();

}

void Exemple::_end() {
    m_actions.finish();

}

void Exemple::update(float dt) {
    m_actions.update(dt);
    shape.update();
}

void Exemple::draw(sf::RenderWindow &win) const {
    win.draw(shape);
    //win.draw(m_star);
}