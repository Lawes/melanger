#ifndef TRANSITION_SCENE_HEADER
#define TRANSITION_SCENE_HEADER

#include "scene.h"
#include <string>

class TransitionScene : public Scene {
    protected:
        sf::Texture m_from, m_to;
        
    public:
        TransitionScene(SceneSwitcher *parent) : Scene(parent) {}
        virtual ~TransitionScene() {}

        void setFromToTexture(const std::string& nextname);

};


#endif