#include "chooselvl.h"
#include "global.h"
#include "Input.h"
#include "sprite.h"
#include "widgets.h"
#include "Box.h"
#include "glFont.h"

using namespace VODZO;
using namespace std;
using namespace gui;

ChooseLvl::ChooseLvl(glFont *f) {
    grid = new GridPanel();
	grid->setNCol(3);
    grid->set_pos(50,50);
    grid->set_espace(0);
    grid->set_intermot(50);
    m_petiteFont = RMa.getFont("smallf");
    m_font = RMa.getFont("f");

}

void ChooseLvl::_update_HightScore() {
    MapWidgets::iterator it = m_guiScore.begin();
    for(; it != m_guiScore.end(); ++it) {

        string txt("Record : ");
        txt += ScoringSystem.bestTime(it->first);
        txt += " ";
        txt += ScoringSystem.bestMove(it->first);
        it->second->setText(txt);

    }
}

void ChooseLvl::_init() {
    SoundManager.launchMusique("choose");
    _update_HightScore();
    m_psm.init();
    m_spray.init();
}

void ChooseLvl::update() {
    m_psm.update();
    m_spray.update();
}

void ChooseLvl::draw() {
    //m_psm.draw();
    m_spray.draw();
    grid->draw();

}

void ChooseLvl::addLvl(const string& name, Sprite& sp, const Compositor& comp) {

    VPanel *vpanel = new VPanel();
    vpanel->add_child( new SimpleText(name, m_font) );
    SpriteW *spw = new SpriteW(sp);
    spw->set_height(140);
    spw->setForegroundDeco(DecoratorManager.get("d5"));
    spw->setSelectDeco(DecoratorManager.get("d7"));
    vpanel->add_child( spw );

    SimpleText *st = new SimpleText("", m_petiteFont);
    st->setColor(Color::grey);

    m_guiScore[name] = st;
    vpanel->add_child( st, HALIGN_LEFT );



    Functor<bool> fun(spw, &Widget::setSelect);
    add_event(MOUSE_MOTION, spw->getBox(), fun);

    add_event(MOUSE_BOUTON, spw->getBox(), comp, SDL_BUTTON_LEFT);

    grid->add_child(vpanel);

}

void ChooseLvl::handle_events(CInput& in) {
    check_events(in);
    if( in.Key(SDLK_e) )
        end();

}

void ChooseLvl::destroy() {
    delete grid;
}
