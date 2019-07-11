
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp> 

#include "gui/widget.h"
#include "libwam/intervals.h"
#include "libwam/random.h"
#include "psm.h"


#include <iostream>

#include "scene_ext.h"
#include "globals.h"

using namespace std;

PresentationScene::PresentationScene(SceneSwitcher *parent) : 
    Scene(parent),
    m_calme(m_context->getBox())
{ }

void PresentationScene::_begin() {
    m_board.start();
    m_time=1.0f;
    //MIXER.play("begining", 100.0f, true);
    m_calme.start();
}

void PresentationScene::_end() {
    m_board.stop();
    m_calme.stop();
    //MIXER.nosound();
}

void PresentationScene::update(float dt) {
    m_time -= dt;
    m_board.update(dt);
    m_calme.update(dt);

    if( m_time < 0.0f) {
        m_board.processRandomMove();
        m_time = Random::Percent()/2;
    }
}

void PresentationScene::draw(sf::RenderTarget &win) const {
    win.draw(m_calme);
    win.draw(m_board);
    win.draw(m_panel);

}

void PresentationScene::load() {
    auto p = GB.beginRun("_randomit");

    auto fullbox = m_context->getBox();
    m_board.init("_randomit", fullbox, 6, 4);

    const auto& rect = m_context->getBox();
    auto center = PSM::Vector(rect.left+rect.width/2, rect.top+rect.height/2);

    sf::Font *f;
    sf::Texture *t;
    gui::Widget *w;

    RM.get("font", f);
    w = new gui::Widget();
    w->setText(L"Mélangeur", *f, 90, sf::Color::Yellow);
    DECOM.apply("txtbg", *w);
    m_panel.add_child(w);
    w = new gui::Widget();
    w->setText(L"le jeu du mélange mortel", *f, 25, sf::Color(200,200,200));
    m_panel.add_child(w);

    m_panel.set_espace(50);
    w = new gui::Widget();
    w->setText("New Game", *f, 50, sf::Color(0,200,0));
    DECOM.apply("underline", *w);
    m_panel.add_child(w);
    add_event(
        sf::Mouse::Left, 
        w->getBox(), 
        [this]{
            cout << "go Challenge" << endl;
            m_context->switchScene(scene::ChooseLvl);
        }
    );
    add_event(
        keymap::gonext,
        [this]{
            cout << "go Challenge" << endl;
            m_context->switchScene(scene::ChooseLvl);
        }
    );
    add_event(w->getBox(), [w]{ w->select();}, [w]{w->unSelect();});

    w = new gui::Widget();
    w->setText("Help", *f, 50, sf::Color(100,100,0));
    DECOM.apply("underline", *w); 
    m_panel.add_child(w);
    add_event(sf::Mouse::Left, w->getBox(), [this]{cout << "help" << endl; m_context->switchScene(scene::GlobalHelp);});
    add_event(w->getBox(), [w]{ w->select();}, [w]{w->unSelect();});

/*
    w = new gui::Widget();
    w->setText("High Scores", *f, 50, sf::Color(150,50,0));
    DECOM.apply("underline", *w); 
    m_panel.add_child(w, gui::HALIGN_LEFT);
    add_event(sf::Mouse::Left, w->getBox(), [this]{cout << "high score" << endl;m_context->switchScene(scene::GlobalScore);});
    add_event(w->getBox(), [w]{ w->select();}, [w]{w->unSelect();});
*/
    w = new gui::Widget();
    w->setText("Quit", *f, 50, colormap::titleColor);
    DECOM.apply("underline", *w); 
    m_panel.add_child(w);
    add_event(sf::Mouse::Left, w->getBox(), [this]{cout << "quit" << endl; m_context->pushOverlay(scene::GlobalExit);});
    add_event(keymap::gonext, [this]{cout << "quit" << endl; m_context->pushOverlay(scene::GlobalExit);});
    add_event(w->getBox(), [w]{ w->select();}, [w]{w->unSelect();});

    auto box = m_context->getBox();
    m_panel.center(box);
    m_panel.update();
}



ExitScreen::ExitScreen(SceneSwitcher *parent) : Scene(parent)
{ }

void ExitScreen::load() {
    sf::Font *f;
    gui::Widget *w;
    RM.get("font", f);
    
    DECOM.apply("lightbg", m_panel);
    m_panel.set_border(10);

    w = new gui::Widget();
    w->setText("Want To Quit ?", *f, 50, colormap::titleColor);
    m_panel.add_child(w);


    gui::HPanel *hp = new gui::HPanel();

    w = new gui::Widget();
    w->setText("Yes", *f, 30);
    w->extend(4);
    hp->add_child(w);
    DECOM.apply("underline", *w);
    add_event(sf::Mouse::Left, w->getBox(), [this]{cout << "yes" << endl; m_context->exit();});
    add_event(keymap::gonext, [this]{cout << "yes" << endl;m_context->exit();});
    add_event(w->getBox(), [w]{ w->select();}, [w]{w->unSelect();});

    w = new gui::Widget();
    w->setText("No", *f, 30);
    w->extend(4);
    hp->set_espace(40);
    hp->add_child(w);
    DECOM.apply("underline", *w);
    add_event(sf::Mouse::Left, w->getBox(), [this]{cout << "no" << endl; end();});
    add_event(keymap::gonext, [this]{cout << "no" << endl; end();});
    add_event(w->getBox(), [w]{ w->select();}, [w]{w->unSelect();});

    m_panel.set_espace(15);
    m_panel.add_child(hp);

    m_panel.center(m_context->getBox());
    m_panel.update();
}

void ExitScreen::_begin() { }
void ExitScreen::_end() { }
void ExitScreen::update(float dt) { }

void ExitScreen::draw(sf::RenderTarget &win) const {
    win.draw(m_panel);
}

EndGameScreen::EndGameScreen(SceneSwitcher *parent) : 
    Scene(parent),
    m_psm(parent->getBox())
{ }

void EndGameScreen::load() {
    m_score=10;
    m_name = "easy";
    m_panel.getRenderBG().setFillColor(sf::Color(100,100,100,50));
}

void EndGameScreen::_begin() {
    sf::Font *f;
    gui::Widget *w;
    RM.get("font", f);

    m_panel.release();
    clearEvents();
    w = new gui::Widget();
    w->setText("Your Score", *f, 50, colormap::titleColor);

    m_panel.add_child(w);
    m_score = GB.getCurrentScore();
    m_name = GB.getGameName();
    w = new gui::Widget();
    w->setText("mode : " + m_name, *f, 25, sf::Color::Green);
    m_panel.set_espace(50);
    m_panel.add_child(w);

    w = new gui::Widget();
    w->setText(to_string(m_score), *f, 25, sf::Color(127,0,255));
    m_panel.set_espace(5);
    m_panel.add_child(w);

    w = new gui::Widget();
    w->setText("Previous Score", *f, 25, sf::Color::Green);
    m_panel.set_espace(20);
    m_panel.add_child(w);

    m_panel.set_espace(2);
    auto scores = GB.getHighScore().getScore(m_name, 10, true);
    for(auto s: scores) {
        w = new gui::Widget();
        w->setText(to_string(s), *f, 25, sf::Color::White);
        m_panel.add_child(w);
    }

    w = new gui::Widget();
    w->setText("Back to menu", *f, 25);
    DECOM.apply("txtbg", *w);
    DECOM.apply("underline", *w);
    m_panel.set_espace(30);
    m_panel.add_child(w);
    add_event(
        sf::Mouse::Left, 
        w->getBox(), 
        [this]{
            cout << "exit" << endl;
            m_context->switchScene(scene::GlobalPresentation);
        }
    );
    add_event(
        keymap::gonext, 
        [this]{
            cout << "exit" << endl;
            m_context->switchScene(scene::GlobalPresentation);
        }
    );
    add_event(w->getBox(), [w]{ w->select();}, [w]{w->unSelect();});

    m_panel.center(m_context->getBox());
    m_panel.update();

    m_psm.start();
    cout << "coucou" << endl;
}
void EndGameScreen::_end() { 
    m_psm.stop();
}

void EndGameScreen::update(float dt) {
    m_psm.update(dt);
}

void EndGameScreen::draw(sf::RenderTarget &win) const {
    win.draw(m_psm);
    win.draw(m_panel);
}

HelpScreen::HelpScreen(SceneSwitcher *parent) : 
    Scene(parent),
    m_calme(m_context->getBox())
{ }

void HelpScreen::_begin() {
    m_calme.start();
    //MIXER.play("begining", 100.0f, true);

}

void HelpScreen::_end() {
    m_calme.stop();
    //MIXER.nosound();
}

void HelpScreen::update(float dt) {
    m_calme.update(dt);
}

void HelpScreen::draw(sf::RenderTarget &win) const {
    win.draw(m_calme);
    win.draw(m_panel);
}

void HelpScreen::load() {
    sf::Font *f;
    sf::Texture *t;

    m_panel.set_espace(30);
    m_panel.set_border(0);
    RM.get("font", f);
    RM.get("help", t);
    gui::Widget *w = new gui::Widget();
    w->getRenderBG().setFillColor(sf::Color::White);
    w->getRenderBG().setTexture(t);
    auto s = t->getSize();
    float r = 800.0f/s.x;
    w->set_size(800,s.y*r);
    m_panel.add_child(w);

    w = new gui::Widget();
    w->setText("Back to menu", *f, 25);
    DECOM.apply("txtbg", *w);
    DECOM.apply("underline", *w);
    m_panel.add_child(w);
    add_event(sf::Mouse::Left, w->getBox(), [this]{cout << "goback" << endl;m_context->switchScene(scene::GlobalPresentation);});
    add_event(keymap::gonext, [this]{cout << "goback" << endl;m_context->switchScene(scene::GlobalPresentation);});
    add_event(w->getBox(), [w]{ w->select();}, [w]{w->unSelect();});

    m_panel.center(m_context->getBox());
    //m_panel.set_pos(0, -65);
    m_panel.update();
}