#ifndef GLOBALS_HEADER
#define GLOBALS_HEADER

#include "gui/decorator.h"
#include "engine/ressourcemanager.h"
#include "engine/mixer.h"
#include "gamebuffer.h"
#include "jukebox.h"

#include <string>

extern gui::DecoratorManager DECOM;
extern RessourceManager RM;

extern GameBuffer GB;

extern Mixer MIXER;

extern Jukebox JUKE;

namespace scene {
    const std::string LaunchGame = "game";
    const std::string GlobalExit = "exit";
    const std::string GlobalPresentation = "presentation";
    const std::string ChooseLvl = "chooselvl";
    const std::string GlobalHelp = "help";
    const std::string GlobalScore = "score";
    const std::string ScoreGame = "scoregame";
    const std::string OverlayBegin = "ready";
}

namespace transition {
    const std::string Fondu = "fondu";
    const std::string Rain = "rain";
    const std::string Verre = "verre";
}

namespace keymap {
    const sf::Keyboard::Key gonext = sf::Keyboard::Space;
}

namespace colormap {
    const sf::Color titleColor = sf::Color(227,69,69);
}


#define MIXER_PLAY(name) MIXER.play(*RM.getSound(name));
#define MIXER_MUSIC(name) MIXER.music(RM.getName(name));
#define MIXER_PLAY_VOL(name, vol) MIXER.play(*RM.getSound(name), vol);

#endif