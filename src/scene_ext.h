#ifndef SCENE_EXTENTION_HEADER
#define SCENE_EXTENTION_HEADER

#include "scene.h"
#include "board.h"
#include "boardshape.h"
#include "psmeffects.h"
#include "tween.h"
#include <SFML/Graphics.hpp>

class PresentationScene : public Scene {
    private:
        gui::VPanel m_panel;
        Board m_board;
        float m_time;
        CalmePsm m_calme;

    public:
        PresentationScene(SceneSwitcher* parent);
        virtual ~PresentationScene() {}

    DERIVED_Scene
};

class EndGameScreen : public Scene {
    private:
        gui::VPanel m_panel;
        FeuxArtifice m_psm;
        std::size_t m_score;
        std::string m_name;
    
    public:
        EndGameScreen(SceneSwitcher* parent);
        virtual ~EndGameScreen() {}

    DERIVED_Scene
};


class ExitScreen : public Scene {
    private:
        gui::VPanel m_panel;
    
    public:
        ExitScreen(SceneSwitcher* parent);
        virtual ~ExitScreen() {}

    DERIVED_Scene
};

class HelpScreen : public Scene {
    private:
        gui::VPanel m_panel;
        CalmePsm m_calme;
    
    public:
        HelpScreen(SceneSwitcher* parent);
        virtual ~HelpScreen() {}

    DERIVED_Scene
};

#endif