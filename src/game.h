#ifndef GAME_HEADER
#define GAME_HEADER
#include "scene.h"
#include "board.h"
#include "boardshape.h"
#include "tween.h"
#include "psmeffects.h"
#include "gui/widget.h"
#include <SFML/Graphics.hpp>
#include <string>

class Game : public Scene {
    public:
        Game(SceneSwitcher* parent);
        virtual ~Game() {}

    private:
        static int _getScore(float t, int nmoves);

        void setGame();
        void build_panel();
        void notifyVictoire();
        void notifyDefaite();

        void panel_endgame(const std::wstring& txt, int totalScore, int time, int nmoves);

        void pause() { m_ispaused=true;}
        bool isPaused() const { return m_ispaused;}
        void resume() { m_ispaused=false;}

        void launch_hint();

    private:
        bool m_ispaused, m_hint;
        float m_time_elapsed;
        Board m_board;
        TweenCollection m_actions;
        sf::Texture m_bg, *cadre;
        sf::FloatRect m_fullbox;
        ZoneObscure m_zobs;
        gui::VPanel m_panel, m_panel_hintquit, m_panel_endgame;
        sf::RectangleShape m_hintshape;

    public:
        int m_currentSelect, m_s1, m_s2;

        void click();

    DERIVED_Scene
};

#endif
