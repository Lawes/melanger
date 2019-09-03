////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp> 

#include <iostream>

#include "libwam/random.h"
#include "engine/sceneswitcher.h"
#include "game.h"
#include "chooselvl.h"
#include "scene_ext.h"
#include "decorator_ext.h"
#include "transition_ext.h"
#include "globals.h"

using namespace std;

void loadRessources() {
    RM.addTexture("fire1", "assets/sprites/fire1.png");
    RM.addTexture("fire2", "assets/sprites/fire2.png");
    RM.addTexture("cadre", "assets/sprites/cadre_board.png");
    RM.addTexture("txtbg", "assets/sprites/bg_txt.png");
    RM.addTexture("underline", "assets/sprites/underline.png");
    RM.addTexture("sun", "assets/sprites/sun1.png");
    RM.addTexture("star", "assets/sprites/star.png");
    RM.addTexture("fuse", "assets/sprites/particule.png");
    RM.addTexture("help", "assets/sprites/help101.png");
    
    RM.addFont("font", "assets/fonts/LiberationMono-Regular.ttf");


    const auto& globalcfg= GB.getGlobalConfig();
    for( const auto& name: globalcfg.getSections()) {
        RM.addTexture(name, GB.getConfig(name).imgfile);
    }

    RM.addSound("bravo", "assets/sounds/bravo.ogg");
	RM.addSound("s1", "assets/sounds/Forge.ogg");
	RM.addSound("s2", "assets/sounds/laser.ogg");
	RM.addSound("s",  "assets/sounds/sabre.ogg");
	RM.addSound("f1", "assets/sounds/fireworks1.ogg");
	RM.addSound("f2", "assets/sounds/fireworks2.ogg");
	RM.addSound("f3", "assets/sounds/fireworks3.ogg");

    RM.addName("finish", "assets/sounds/BitTripRunner.ogg");
    RM.addName("m_forGame1", "assets/sounds/BTR_Impetus.ogg");
    RM.addName("m_forGame2", "assets/sounds/BTR_Strength.ogg");
    RM.addName("m_forGame3", "assets/sounds/JOST_SecretMinesofNewMadrid.ogg");
    RM.addName("m_forGame4", "assets/sounds/MACHINA_GameboyTune.ogg");
    RM.addName("intro", "assets/sounds/JOST_Intro.ogg");
    RM.addName("brule", "assets/sounds/cabrule.ogg");
    RM.addName("choose", "assets/sounds/JOST_Epilogue.ogg");

    JUKE.add_tag("m_forGame1");
    JUKE.add_tag("m_forGame2");
    JUKE.add_tag("m_forGame3");
    JUKE.add_tag("m_forGame4");

}

void loadScenes(SceneSwitcher& app, sf::FloatRect& fullbox) {
    app.newScene<ChooseLvl>(scene::ChooseLvl);
    app.newScene<PresentationScene>(scene::GlobalPresentation);
    app.newScene<Game>(scene::LaunchGame);
    app.newScene<ExitScreen>(scene::GlobalExit);
    app.newScene<EndGameScreen>(scene::ScoreGame);
    app.newScene<HelpScreen>(scene::GlobalHelp);
    app.newTransition<FonduTransition>(transition::Fondu);
    app.newTransition<RainTransition>(transition::Rain);
    app.newTransition<VerreTransition>(transition::Verre);
}

int main()
{
    // Define some constants
    const int gameWidth = 1000;
    const int gameHeight = 680;

    // Create the window of the application
    sf::RenderWindow window(sf::VideoMode(gameWidth, gameHeight, 32), "Melanger",
                            sf::Style::Titlebar | sf::Style::Close);
    window.setVerticalSyncEnabled(true);

    MIXER.setVolume(20);

    sf::FloatRect nullbox, fullbox(0, 0, gameWidth, gameHeight);

    sf::View view(fullbox);
    window.setView(view);

    Random();
    loadRessources();
    loadDecorators();

    SceneSwitcher app(window);

    loadScenes(app, fullbox);

    app.switchScene(scene::GlobalPresentation);

    while (window.isOpen() && !app.wantExit())
    {
        JUKE.check();
        app.run();
    }

    return EXIT_SUCCESS;
}