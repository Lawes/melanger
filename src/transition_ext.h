#ifndef TRANSITION_SCENE_EXTENTION
#define TRANSITION_SCENE_EXTENTION

#include "engine/transitionscene.h"
#include "engine/sceneswitcher.h"


class TestTansition : public TransitionScene {
    public:
        TestTansition(SceneSwitcher *p) : TransitionScene(p, 3) {}
        virtual ~TestTansition() {}

    DERIVED_Scene
};

class FonduTransition : public TransitionScene {
    private:
        sf::RectangleShape m_before, m_after;
        sf::Color m_color;
    public:
        FonduTransition(SceneSwitcher *p) : TransitionScene(p, 3) {}
        virtual ~FonduTransition() {}

    DERIVED_Scene
};

/*
class RainTransition : public TransitionScene {
    private:

        static const int SizeX;
        static const int SizeY;

        struct SpriteContainer {
            Sprite sp;
            VODZO::Vector2 pos;
            float angleZ, alpha;
        };

        std::vector< SpriteContainer > m_listeSprite;
        std::vector< int > m_ordre, to_plot;
        int m_current_sp;

        Sprite sp_bg;
        float dxf, dyf;

        Organizer m_mixer;

        void load(Scene *sc_before, Scene *sc_after);
        void _captureScreen(SDL_Surface *img1);

        virtual void _init();

    public:
        RainTransition(float t, SceneSwitcher *context) : Transition(t) {
                setContext(context);
                m_listeSprite.resize(SizeX*SizeY);

                dxf = static_cast<float>(Windows_width)/SizeX;
                dyf = static_cast<float>(Windows_height)/SizeY;
                m_current_sp = SizeX*SizeY;
            }
        virtual void draw();
        virtual void update();

*/
/*

class VerreTransition : public Transition {
    static const int SizeX;
    static const int SizeY;
  public:
    VerreTransition(float t, SceneSwitcher *context) :
        Transition(t) {
            setContext(context);
            m_listeSprite.resize(SizeX*SizeY);

            dxf = static_cast<float>(Windows_width)/SizeX;
            dyf = static_cast<float>(Windows_height)/SizeY;

        }

    virtual void draw();
    virtual void update();

  private:
    void load(Scene *sc_before, Scene *sc_after);

    float dxf, dyf;

    struct SpriteContainer {
        Sprite sp[2];
        int posX, posY;
        float angleZ, alpha;
    };
    std::vector< SpriteContainer > m_listeSprite;
    std::vector< int > m_ordre;

    virtual void _init();

    Organizer m_mixer;

    void _captureScreen(SDL_Surface *img1,
                        SDL_Surface *img2);

};


class RainTransition : public Transition {
    private:

        static const int SizeX;
        static const int SizeY;

        struct SpriteContainer {
            Sprite sp;
            VODZO::Vector2 pos;
            float angleZ, alpha;
        };

        std::vector< SpriteContainer > m_listeSprite;
        std::vector< int > m_ordre, to_plot;
        int m_current_sp;

        Sprite sp_bg;
        float dxf, dyf;

        Organizer m_mixer;

        void load(Scene *sc_before, Scene *sc_after);
        void _captureScreen(SDL_Surface *img1);

        virtual void _init();

    public:
        RainTransition(float t, SceneSwitcher *context) : Transition(t) {
                setContext(context);
                m_listeSprite.resize(SizeX*SizeY);

                dxf = static_cast<float>(Windows_width)/SizeX;
                dyf = static_cast<float>(Windows_height)/SizeY;
                m_current_sp = SizeX*SizeY;
            }
        virtual void draw();
        virtual void update();
};
*/

#endif