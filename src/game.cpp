#include <iostream>
#include <string>
#include "Input.h"
#include "global.h"
#include "game.h"
#include "board.h"
#include "glFont.h"
#include "Color.h"
#include "popup.h"
#include "sprite.h"
#include "transition.h"

using namespace VODZO;
using namespace std;
using gui::Box;

Game::Game(const string& txt) : m_id(txt) {
    string file;
    BoardConf.getInfo(txt, file, ImageConfig::nWidth,
                ImageConfig::nHeight, ImageConfig::sizeTexture);


    m_cadre.x = 0;
    m_cadre.y = 45;
    m_cadre.w = Windows_width;
    m_cadre.h = Windows_height-45;

    board.init(file, m_cadre);

    m_font = RMa.getFont("f");

    m_help_pu = new HelpPopup(SpritesManager.get(txt), m_cadre);
    m_help_pu->setDeco(DecoratorManager.get("d4"));

}

Game::~Game() {
    SoundManager.stop();
    delete m_help_pu;
}

void Game::_init() {
    Sequence *seq = new Sequence();
    Parallele * par = new Parallele();
    m_x = 0;
    m_y = 600;

    par->add( new TweenerLin<int>(&m_x, 700, 3));
    par->add( new TweenerLin<int>(&m_y, 0, 3));
    col = Color(0,0,0,1);
    par->add( new TweenerLin<Color>(&col, Color(1,1,1,1), 3));

    seq->add( new Wait(4));
    seq->add( new Func(BindFirst(Functor<string>(this, &Game::setTmp), "coucou")));
    seq->add( new Wait(4));
    seq->add( new Func(BindFirst(Functor<string>(this, &Game::setTmp), "ta mère")));
    seq->add( par);
    seq->add( new Func(BindFirst(Functor<string>(this, &Game::setTmp), "coucou")));

    intervalsManager.add(seq);

    m_x = 700;
    m_y = 0;
    col = Color::white;

    m_chrono.start();
	SoundManager.launchMusique();

	m_bg.init();
}

void Game::update() {
    VODZO::Timer::toStr(m_chrono.top(), m_timeTxt);
    intervalsManager.update();

    board.update();

    m_bg.update();

    if( board.finish() ) {
        int res = ScoringSystem.checkScore(m_id, m_chrono.top(), board.getNbMoves());
        ScoringSystem.addScore(m_id, m_chrono.top(), board.getNbMoves());
        ResultGame *rg = new ResultGame(m_id, m_cadre);
        rg->setScore(m_chrono.top(), board.getNbMoves());
        m_context->addScene(rg);
        m_context->addScene(new FonduTransition(3.0, m_context) );
        end();
    }
}

void Game::draw() {
    char buffer[5];

    m_bg.draw();

    glColor3d(1,1,1);
    sprintf(buffer, "Moves : %3d", board.getNbMoves());
    m_font->print(400,0, buffer);
    m_font->print(0,0, "time : " + m_timeTxt);

    board.draw();


    glColor3fv(&col.r);
    m_font->print(m_x,m_y, m_tmpStr);
}

void Game::handle_events(VODZO::CInput& in) {
    if( in.Key(SDLK_h) ) {
        in.unKey(SDLK_h);
        m_context->setPopup(m_help_pu);

    }

    board.handle_events(in);
}

