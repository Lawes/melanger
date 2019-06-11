////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp> 

#include <iostream>

#include "random.h"
#include "sceneextention.h"
#include "decorator_ext.h"
#include "globals.h"
#include "game.h"

using namespace std;

void loadRessources() {
    RM.addTexture("pasteque", "assets/sprites/pasteque.png");
    RM.addTexture("raisin", "assets/sprites/raisin.png");
    RM.addTexture("elroy", "assets/sprites/ghostElroy.png");
    RM.addTexture("trophy", "assets/sprites/trophy.png");
    RM.addTexture("sansvie", "assets/sprites/ensembleVide.png");

    RM.addTexture("fire1", "assets/sprites/fire1.png");
    RM.addTexture("fire2", "assets/sprites/fire2.png");
    RM.addTexture("globe", "assets/sprites/globe.png");
    RM.addTexture("pacmanLifes", "assets/sprites/pacmanHR.png");
    RM.addTexture("ghost1", "assets/sprites/ghost1.png");
    RM.addTexture("tsol","assets/sprites/sol_laby.png");
    RM.addTexture("tdot","assets/sprites/dot_laby.png");
    RM.addTexture("tdoor","assets/sprites/door_laby.png");
    RM.addTexture("bubble", "assets/sprites/bubble.png");
    RM.addTexture("sun", "assets/sprites/sun1.png");
    RM.addTexture("star", "assets/sprites/star.png");
    RM.addTexture("fuse", "assets/sprites/particule.png");

    RM.addTexture("circleBG", "assets/sprites/bgCircle.png");
    RM.addTexture("barBG", "assets/sprites/bar.png");
    RM.addTexture("txtbg", "assets/sprites/bg_txt.png");

    RM.addTexture("murlaby", "assets/sprites/mur_laby.png");
    RM.addTexture("underline", "assets/sprites/underline.png");
    RM.addTexture("turbocadre", "assets/sprites/turboCadre.png");

    RM.addTexture("help", "assets/sprites/help.png");

    RM.addAnimations("assets/animationconfig.txt");
    RM.addFont("font", "assets/fonts/appleberry_with_cyrillic.ttf");

    RM.addSound("paceat", "assets/sounds/pacman_eatmiam.ogg");
    RM.addSound("paceatfruit", "assets/sounds/pacman_eatfruit.ogg");
    RM.addSound("paceatghost", "assets/sounds/pacman_eatghost.ogg");
	RM.addSound("f1", "assets/sounds/fireworks1.ogg");
	RM.addSound("f2", "assets/sounds/fireworks2.ogg");
	RM.addSound("f3", "assets/sounds/fireworks3.ogg");
	RM.addSound("inter", "assets/sounds/pacman_intermission.ogg");
    RM.addSound("death", "assets/sounds/pacman_death.ogg");
    RM.addSound("rampe", "assets/sounds/pacman_chomp.ogg");
    RM.addSound("bip", "assets/sounds/bip.ogg");
    RM.addSound("lastbip", "assets/sounds/lastbip.ogg");
    RM.addSound("bravo", "assets/sounds/bravo.ogg");
    RM.addSound("begining", "assets/sounds/pacman_beginning.ogg");
}

void loadScenes(SceneSwitcher& app, sf::FloatRect& fullbox) {
    app.newScene<EndGameScreen>(scene::ScoreGame);
    app.newScene<BeginGameScreen>(scene::OverlayBegin);
    app.newScene<ExitScreen>(scene::GlobalExit);
    app.newScene<PresentationScene>(scene::GlobalPresentation);
    app.newScene<AllScoreScreen>(scene::GlobalScore);
    app.newScene<HelpScreen>(scene::GlobalHelp);
    app.newScene<Game>(scene::LaunchGame);
}

int main()
{
    // Define some constants
    const int gameWidth = 560;
    const int gameHeight = 735;

    // Create the window of the application
    sf::RenderWindow window(sf::VideoMode(gameWidth, gameHeight, 32), "Pacman",
                            sf::Style::Titlebar | sf::Style::Close);
    window.setVerticalSyncEnabled(true);

    MIXER.setVolume(30);

    int dy = 75;
    sf::FloatRect nullbox, fullbox(0,-dy,gameWidth,  gameHeight);

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
        app.run();
    }

    return EXIT_SUCCESS;
}