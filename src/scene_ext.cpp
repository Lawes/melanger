
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
    m_fullbox = m_context->getBox();
    sf::Image *img;
    RM.get("test", img);
    RM.get("cadre", cadre);

    bg.loadFromImage(*img);

    m_board.init("test", m_fullbox, 4, 4);
    

    shape.setTexture(&bg, cadre);

    shape.init(0,0,0, sf::Vector2f(200,200), 200);

    auto s = bg.getSize();

    shape.setTextcoords(sf::FloatRect(0,0, s.x/3, s.y/3 ));

    m_actions.To(0, shape.getTranslate(), sf::Vector2f(400,400), 10);
    m_actions.To(1, shape.getRotate(), 360.0f, 10);

    add_event(
        m_fullbox,
        [this]{
            auto &in = m_context->getInput();
            auto current = m_board.findshape(in.MouseX(), in.MouseY());
            if(current >=0) {
                m_currentSelect = current;
                m_board.getShape(m_currentSelect).select(true);
                cout << "select" << endl;
            }
            if( m_currentSelect != current) {
                m_board.getShape(m_currentSelect).select(false);
                m_currentSelect = current;
                cout << "unselect" << endl;
            }
        },
        []{}
    );

    add_event(
        sf::Mouse::Button::Left,
        m_fullbox,
        [this]{
            if( m_currentSelect>=0) {
                m_s1 = m_currentSelect;
            }
        }
    );
}

void Exemple::_begin() {
    m_actions.start();
    m_board.start();
}

void Exemple::_end() {
    m_actions.finish();
    m_board.stop();
}

void Exemple::update(float dt) {
    m_actions.update(dt);
    shape.update();
    m_board.update(dt);
}

void Exemple::draw(sf::RenderWindow &win) const {
    win.draw(shape);
    win.draw(m_board);
    //win.draw(m_star);
}