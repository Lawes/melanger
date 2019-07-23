#ifndef TRANSITION_SCENE_HEADER
#define TRANSITION_SCENE_HEADER

#include "scene.h"
#include <string>

class TransitionScene : public Scene {
    protected:
        float m_totaltime, m_currenttime, m_p;
        sf::Texture m_from, m_to;

        void beginTransitionTime();
        bool updateTransitionTime(float dt)    ;

    
    public:
        TransitionScene(SceneSwitcher *parent, float t) : 
            Scene(parent),
            m_totaltime(t),
            m_currenttime(0),
            m_p(0)
        {}
        virtual ~TransitionScene() {}

        void setFromToTexture(const std::string& nextname);

};


#endif