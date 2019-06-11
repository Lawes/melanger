#ifndef GAME_HEADER
#define GAME_HEADER

#include <string>
#include "board.h"
#include "scene.h"
#include "timer.h"
#include "intervals.h"
#include "Color.h"
#include "Box.h"
#include "psmEffects.h"

class HelpPopup;
class CompteBG;
class Sprite;

namespace VODZO {
    class CInput;
    class glFont;
}



class Game : public Scene {
public:
    Game(const std::string& name);
    virtual ~Game();
    void update();
    void draw();
    void handle_events(VODZO::CInput& in);


private:
    virtual void _init();
    std::string m_id;
    Board board;
    VODZO::glFont *m_font;
    VODZO::Timer m_chrono;
    std::string m_timeTxt;
    std::string m_tmpStr;
    int m_x, m_y;
    VODZO::Color col;

    gui::Box m_cadre;

    HelpPopup *m_help_pu;

    Organizer intervalsManager;
    ZoneObscure m_bg;


    void setTmp(const std::string& txt) { m_tmpStr = txt; }


};



#endif
