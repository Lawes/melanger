#ifndef TRANSITION_SCENE_EXTENTION
#define TRANSITION_SCENE_EXTENTION

#include "engine/transitionscene.h"
#include "engine/sceneswitcher.h"
#include "engine/shapecontainer.h"
#include "tween.h"

class FonduTransition : public TransitionScene {
    private:
        sf::RectangleShape m_before, m_after;
        sf::Color m_color;
    public:
        FonduTransition(SceneSwitcher *p) : TransitionScene(p, 2) {}
        virtual ~FonduTransition() {}

    DERIVED_Scene
};

class RainTransition : public TransitionScene {
    private:
        int m_sizex, m_sizey;
        ShapeContainer m_shapes1, m_shapes2;
        TweenCollection m_actions;

    public:
        RainTransition(SceneSwitcher *p) : 
            TransitionScene(p, 2)
        {}

        virtual ~RainTransition() {}

    DERIVED_Scene
};

class VerreTransition : public TransitionScene {
    private:
        int m_sizex, m_sizey;
        sf::VertexArray m_va;
        ShapeContainer m_shapes1, m_shapes2;
        TweenCollection m_actions;

    public:
        VerreTransition(SceneSwitcher *p) : 
            TransitionScene(p, 2),
            m_va(sf::Lines)
        {}

        virtual ~VerreTransition() {}

    DERIVED_Scene
};

#endif