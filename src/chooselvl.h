#ifndef CHOOSELVL_HEADER
#define CHOOSELVL_HEADER

#include "scene.h"
#include "gui/widget.h"
#include "psmeffects.h"
#include <SFML/Graphics.hpp>


class ChooseLvl : public Scene {
    private:
        gui::VPanel m_panel;
        //ZoneObscure m_zobs;
        CalmePsm m_calme;
    public:
        ChooseLvl(SceneSwitcher* parent);
        virtual ~ChooseLvl() {}

    DERIVED_Scene
};

#endif
