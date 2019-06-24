
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp> 

#include "intervals.h"
#include "psm.h"


#include <iostream>

#include "scene_ext.h"
#include "globals.h"

using namespace std;


Exemple::Exemple(SceneSwitcher *parent) : 
    Scene(parent),
    m_currentSelect(-1),
    m_s1(-1),
    m_s2(-1)
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
        sf::Mouse::Button::Left,
        m_fullbox,
        [this]{ click(); }
    );
    add_event(
        sf::Keyboard::A,
        [this]{ 
            if( m_currentSelect != -1)
                m_board.processRotation(m_currentSelect, Board::SensRotation::Moins);
        }
    );
    add_event(
        sf::Keyboard::E,
        [this]{
            if( m_currentSelect != -1)
                m_board.processRotation(m_currentSelect, Board::SensRotation::Plus);
        }
    );
}

void Exemple::click() {
    if( m_currentSelect<0)
        return;

    if( m_s1 == m_currentSelect) {
        if( m_s2 != -1) {
            m_board.getShape(m_s1).select(false);
            m_s1 = m_s2;
            m_s2 = -1;
        }
        else {
            m_board.getShape(m_s1).select(false);
            m_s1 = -1;
        }
    }
    else if( m_s2 == m_currentSelect) {
        m_board.getShape(m_s2).select(false);
        m_s2 = -1;
    }
    else if(m_s1 == -1) {
        m_s1 = m_currentSelect;
        m_board.getShape(m_currentSelect).select(true);
    }
    else if( m_s1 != -1 && m_s2 == -1) {
        m_s2 = m_currentSelect;
        //m_board.getShape(m_currentSelect).select(true);
        m_board.getShape(m_s1).select(false);
        m_board.processEchange(m_s1, m_s2);
        m_s1=-1;
        m_s2=-1;
    }
}

void Exemple::_begin() {
    m_currentSelect = -1;
    m_s1=-1;
    m_s2=-1;
    m_actions.start();
    m_board.start();
}

void Exemple::_end() {
    m_actions.finish();
    m_board.stop();
}

void Exemple::update(float dt) {
    auto &in = m_context->getInput();
    auto current = m_board.findshape(in.MouseX(), in.MouseY());
    if( m_currentSelect != current && m_currentSelect>=0) {
        m_board.getShape(m_currentSelect).overMouse(false);
        m_currentSelect = -1;
    }
    if(current >=0) {
        m_board.getShape(current).overMouse(true);
        m_currentSelect = current;
    }

    m_actions.update(dt);
    shape.update();
    m_board.update(dt);
}

void Exemple::draw(sf::RenderWindow &win) const {
    win.draw(shape);
    win.draw(m_board);
    //win.draw(m_star);
}