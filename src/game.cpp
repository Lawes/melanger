#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp> 

#include "intervals.h"
#include "psm.h"


#include <iostream>

#include "game.h"
#include "globals.h"

using namespace std;


Game::Game(SceneSwitcher *parent) : 
    Scene(parent),
    m_currentSelect(-1),
    m_s1(-1),
    m_s2(-1),
    m_zobs(m_context->getBox())
{ }

void Game::load() {
    m_fullbox = m_context->getBox();
    m_fullbox.width -= 300;
    sf::Image *img;
    RM.get("test", img);
    RM.get("cadre", cadre);

    bg.loadFromImage(*img);

    m_board.init("test", m_fullbox, 4, 4);

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

void Game::setGame() {

}

void Game::build_panel() {

}

void Game::click() {
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

void Game::_begin() {
    m_currentSelect = -1;
    m_s1=-1;
    m_s2=-1;
    m_actions.start();
    m_board.start();
    m_zobs.start();
}

void Game::_end() {
    m_actions.finish();
    m_board.stop();
    m_zobs.stop();
}

void Game::update(float dt) {
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
    m_board.update(dt);
    m_zobs.update(dt);
}

void Game::draw(sf::RenderWindow &win) const {
    win.draw(m_zobs);
    win.draw(m_board);
    //win.draw(m_star);
}