#ifndef CHOOSELVL_HEADER
#define CHOOSELVL_HEADER

#include "scene.h"
#include "widgets.h"
#include "eventsDispatcher.h"
#include "psmEffects.h"
#include <string>


class Sprite;
namespace VODZO {
    class glFont;
}

class ChooseLvl : public Scene, private EventsDispatcher {
    public:
    ChooseLvl(VODZO::glFont *f);
    virtual ~ChooseLvl() {}

    void update();
    void draw();

    void destroy();

    void addLvl(const std::string& name, Sprite& sp, const Compositor& comp);
    void handle_events(VODZO::CInput& in);

    private:
        //typedef std::pair<gui::SimpleText*,gui::SimpleText*> WidgetScore;
        typedef gui::SimpleText* WidgetScore;
        typedef std::map<std::string, WidgetScore> MapWidgets;
        gui::GridPanel *grid;
        VODZO::glFont *m_font, *m_petiteFont;
        MapWidgets m_guiScore;

        void _init();


        void _update_HightScore();

        ZoneObscure m_psm;
        CalmePsm m_spray;


};

#endif
