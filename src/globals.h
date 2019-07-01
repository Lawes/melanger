#ifndef GLOBALS_HEADER
#define GLOBALS_HEADER

#include "gui/decorator.h"
#include "engine/ressourcemanager.h"
#include "gamebuffer.h"

#include <string>

extern gui::DecoratorManager DECOM;
extern RessourceManager RM;

extern GameBuffer GB;


namespace scene {
    const std::string LaunchGame = "game";
    const std::string GlobalExit = "exit";
    const std::string GlobalPresentation = "presentation";
    const std::string GlobalHelp = "help";
    const std::string GlobalScore = "score";
    const std::string ScoreGame = "scoregame";
    const std::string OverlayBegin = "ready";
}

namespace keymap {
    const sf::Keyboard::Key gonext = sf::Keyboard::Space;
}

namespace colormap {
    const sf::Color titleColor = sf::Color(227,69,69);
}
#endif