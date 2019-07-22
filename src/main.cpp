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
}

void loadScenes(SceneSwitcher& app, sf::FloatRect& fullbox) {
    app.newScene<ChooseLvl>(scene::ChooseLvl);
    app.newScene<PresentationScene>(scene::GlobalPresentation);
    app.newScene<Game>(scene::LaunchGame);
    app.newScene<ExitScreen>(scene::GlobalExit);
    app.newScene<EndGameScreen>(scene::ScoreGame);
    app.newScene<HelpScreen>(scene::GlobalHelp);
    app.newTransition<TestTansition>("trans");
    app.newTransition<FonduTransition>(transition::Fondu);
    app.newTransition<RainTransition>(transition::Rain);
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

    //MIXER.setVolume(30);

    sf::FloatRect nullbox, fullbox(0, 0, gameWidth, gameHeight);

    sf::View view(fullbox);
    window.setView(view);

    Random();
    loadRessources();
    loadDecorators();

    SceneSwitcher app(window);

    loadScenes(app, fullbox);
    GB.beginRun("test");

    app.switchScene(scene::GlobalPresentation);

    while (window.isOpen() && !app.wantExit())
    {
        app.run();
    }

    return EXIT_SUCCESS;
}