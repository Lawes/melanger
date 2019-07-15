#ifndef TRANSITION_SCENE_EXTENTION
#define TRANSITION_SCENE_EXTENTION

#include "engine/transitionscene.h"
#include "engine/sceneswitcher.h"


class TestTansition : public TransitionScene {

    public:
        TestTansition(SceneSwitcher *p) : TransitionScene(p) {}
        virtual ~TestTansition() {}

    DERIVED_Scene
};

#endif