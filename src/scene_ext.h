#ifndef SCENE_EXTENTION_HEADER
#define SCENE_EXTENTION_HEADER

#include "scene.h"
#include "board.h"
#include "boardshape.h"
#include "tween.h"
#include <SFML/Graphics.hpp>

class Exemple : public Scene {
    public:
        Exemple(SceneSwitcher* parent);
        virtual ~Exemple() {}

    private:
        Board m_board;
        TweenCollection m_actions;
        sf::Texture bg, *cadre;
        BoardShape shape;


    DERIVED_Scene

};

#endif