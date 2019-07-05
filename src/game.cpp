#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp> 

#include "intervals.h"
#include "psm.h"

#include "gui/widget.h"
#include "libwam/timeit.h"

#include <iostream>
#include "globals.h"
#include "game.h"
#include "globals.h"

using namespace std;


Game::Game(SceneSwitcher *parent) : 
    Scene(parent),
    m_ispaused(false),
    m_hint(false),
    m_time_elapsed(0),
    m_currentSelect(-1),
    m_s1(-1),
    m_s2(-1),
    m_zobs(m_context->getBox())
{ }

int Game::_getScore(float t, int nmoves) {
    return static_cast<int>(t*10.0f) + nmoves*2;
}

void Game::load() {
    m_fullbox = m_context->getBox();
    m_fullbox.width -= 300;

    RM.get("cadre", cadre);

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

    sf::Font *f;
    RM.get("font", f);
    gui::Widget *w = new gui::Widget();
    w->setText("Hint", *f, 30, sf::Color::White);
    DECOM.apply("txtbg", *w);
    DECOM.apply("underline", *w);
    add_event(w->getBox(), [w]{ w->select();}, [w]{w->unSelect();});
    add_event(
        sf::Mouse::Left,
        w->getBox(),
        [this]{
            if( !m_hintshape.isSelected())
                launch_hint(true);
            else
                launch_hint(false);
            
        }
    );
    m_panel_hintquit.set_espace(100);
    m_panel_hintquit.add_child(w);

    w = new gui::Widget();
    w->setText("Resume game", *f, 30, sf::Color::White);
    DECOM.apply("txtbg", *w);
    DECOM.apply("underline", *w);
    add_event(w->getBox(), [w]{ w->select();}, [w]{w->unSelect();});
    m_panel_hintquit.add_child(w, gui::HALIGN_LEFT);

    m_panel_hintquit.set_pos(0, m_fullbox.top+m_fullbox.height/2);
    m_panel_hintquit.juxtaposeHorizontal(m_fullbox, gui::HALIGN_RIGHT,0);

    m_panel_hintquit.update();

    DECOM.apply("lightbg", m_panel_endgame);
    m_panel_endgame.unVisible();
}

void Game::setGame() {
    sf::Image *img;
    RM.get("test", img);
    m_bg.loadFromImage(*img);

    m_board.init("test", m_fullbox, 4, 4);

    sf::Vector2f v(m_fullbox.left+m_fullbox.width/2, m_fullbox.top+m_fullbox.height/2);
    m_hintshape.setTransform(v, 0, 0);
    auto s = m_bg.getSize();
    m_hintshape.setTexture(&m_bg, cadre);
    m_hintshape.setTextcoords(sf::FloatRect(0,0,s.x, s.y));
    m_hintshape.select(false);

}

void Game::launch_hint(bool open) {
    m_hintshape.select(true);

    auto *s = new Sequence();
    if( open) {
        s->add( new TweenerLin<float>(m_hintshape.getScale(), 0.0f, 250.0f, 1.0f) );
        s->add( new IntervalAction(5.0f, []{cout << "coucou hint" << endl;}) );
    }
    else{
        s->add( new TweenerLin<float>(m_hintshape.getScale(), 0.0f, 1.0f) );
        s->add( new IntervalAction([this]{m_hintshape.select(false);}) );
    }
    m_actions.add(0, s);
    s->start();
}

void Game::build_panel() {
    sf::Font *f;
    m_panel.release();
    m_panel.set_espace(10);
    RM.get("font", f);

    auto name = GB.getGameName();

    gui::Widget *w = new gui::Widget();
    w->setText(name, *f, 50, sf::Color::Yellow);
    m_panel.add_child(w, gui::HALIGN_LEFT);

    gui::HPanel *hp = new gui::HPanel();
    w = new gui::Widget();
    w->setText("Time : ", *f, 30, sf::Color::Blue);
    hp->add_child(w);
    w = new gui::Widget();
    w->setText(Timeit::to_string(m_time_elapsed), *f, 30, sf::Color::Magenta);
    hp->add_child(w);
    m_panel.add_child(hp, gui::HALIGN_LEFT);

    hp = new gui::HPanel();
    w = new gui::Widget();
    w->setText("Moves : ", *f, 30, sf::Color::Blue);
    hp->add_child(w);
    w = new gui::Widget();
    w->setText(to_string(m_board.getNbMoves()), *f, 30, sf::Color::Magenta);
    hp->add_child(w);
    m_panel.add_child(hp, gui::HALIGN_LEFT);

    m_panel.juxtaposeHorizontal(m_fullbox, gui::HALIGN_RIGHT, 5);
    m_panel.update();
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
    setGame();

    m_time_elapsed = 0.0;
    m_currentSelect = -1;
    m_s1=-1;
    m_s2=-1;
    m_actions.start();
    m_board.start();
    m_zobs.start();
    resume();
}

void Game::_end() {
    m_actions.finish();
    m_board.stop();
    m_zobs.stop();
    pause();
}

void Game::update(float dt) {
    m_time_elapsed += dt;
    m_zobs.update(dt);
    m_hintshape.update();

    m_actions.update(dt);

    if( isPaused() )
        return;

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

    m_board.update(dt);


    build_panel();

    if( m_board.isFinished() ) {
        m_actions.clear();

        m_actions.add( new IntervalAction(
            [this]{
                pause();
                notifyVictoire();
            }, 
            7.0f,
            [this]{
                cout << "end score" << endl;
                m_panel_endgame.setVisible(false);
                m_context->switchScene(scene::ScoreGame);
            }
        ));
        m_actions.start();
    }
}

void Game::notifyVictoire() {
    //MIXER.play("bravo");
    auto nmoves=m_board.getNbMoves();
    int score = _getScore(m_time_elapsed, nmoves);
    GB.saveScore(score);
    cout << "show end score" << endl;
    panel_endgame(L"Victoire !", score, m_time_elapsed, nmoves);
}

void Game::notifyDefaite() {
    panel_endgame(L"Défaite :(", 0, 0, 0);
}


void Game::panel_endgame(const wstring& txt, int totalScore, int time, int nmoves) {
    m_panel_endgame.release();

    gui::Widget *w = new gui::Widget();
    sf::Font *f;
    RM.get("font", f);
    w->setText(txt, *f, 50, sf::Color(227,69,69));
    m_panel_endgame.add_child(w);

    gui::VPanel *vp = new gui::VPanel();
    vp->set_espace(2);
    vp->set_border(2);

    gui::HPanel *hp = new gui::HPanel();
    w = new gui::Widget();
    w->setText("Time ", *f, 35);
    hp->add_child(w);

    w = new gui::Widget();
    w->setText("+ " + to_string(time), *f, 35, sf::Color(255,0,255));
    hp->add_child(w);
    vp->add_child(hp);

    hp = new gui::HPanel();
    w = new gui::Widget();
    w->setText("Moves ", *f, 35);
    hp->add_child(w);

    w = new gui::Widget();
    w->setText("+ " + to_string(nmoves), *f, 35, sf::Color(255,0,255));      
    hp->add_child(w);
    vp->add_child(hp);

    hp = new gui::HPanel();
    w = new gui::Widget();
    w->setText("   Score : ", *f, 35);
    hp->add_child(w);

    w = new gui::Widget();
    w->setText( to_string(totalScore), *f, 35, sf::Color(255,0,255));   
    hp->add_child(w);
    vp->add_child(hp);

    m_panel_endgame.add_child(vp);

    m_panel_endgame.center(m_context->getBox());
    m_panel_endgame.setVisible(true);
    m_panel_endgame.update();
}

void Game::draw(sf::RenderWindow &win) const {
    win.draw(m_zobs);
    win.draw(m_board);

    win.draw(m_panel);
    win.draw(m_panel_hintquit);
    win.draw(m_panel_endgame);

    if( m_hintshape.isSelected())
        win.draw(m_hintshape);
    //win.draw(m_star);
}