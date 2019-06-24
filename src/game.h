#ifndef GAME_HEADER
#define GAME_HEADER
#include "scene.h"
#include "board.h"
#include "boardshape.h"
#include "tween.h"
#include "psmeffects.h"
#include "gui/widget.h"
#include <SFML/Graphics.hpp>

class Game : public Scene {
    public:
        Game(SceneSwitcher* parent);
        virtual ~Game() {}

    private:
        void setGame();
        void build_panel();

    private:
        Board m_board;
        TweenCollection m_actions;
        sf::Texture bg, *cadre;
        sf::FloatRect m_fullbox;
        ZoneObscure m_zobs;
        gui::VPanel m_panel;

    public:
        int m_currentSelect, m_s1, m_s2;

        void click();


    DERIVED_Scene

};

#endif
