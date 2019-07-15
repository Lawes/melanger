#include "transition_ext.h"


void TestTansition::_begin() {

}

void TestTansition::_end() {

}

void TestTansition::update(float dt) {

}

void TestTansition::draw(sf::RenderTarget &win) const {
    sf::RectangleShape s1, s2;

    s1.setSize(sf::Vector2f(200,200));
    s2.setSize(sf::Vector2f(200,200));


    s1.setTexture(&m_from);
    s2.setTexture(&m_to);

    s1.setPosition(0,0);
    s2.setPosition(300,300);

    win.draw(s1);
    win.draw(s2);
}

void TestTansition::load() {}