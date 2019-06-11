#ifndef BOARD_HEADER
#define BOARD_HEADER

#include <vector>
#include <string>
#include <list>
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

#include "Vector2.h"
#include "state.h"
#include "tweenId.h"
#include "shape.h"

namespace VODZO {
    class CInput;
    class Vector2;
}

namespace gui {
    class Box;
}



class Board {

    public:
        Board(): tween(), select1(-1), select2(-1), select(-1), pressed(false), m_nMoves(0), m_taille(10) {}
        virtual ~Board();

        void init(const std::string& fileName, const gui::Box& box);
        void handle_events(VODZO::CInput &in);

        virtual void update();

        bool finish();

        void draw();
        void clear();

        int getNbMoves() const { return m_nMoves;}

    protected:
        std::vector<Shape> listeShape;
        std::vector<State> states;
        std::list<int> ordre;

        VODZO::TweenId tween;

        int select1, select2, select;

        bool pressed;

        void lastMove(const int id);
        void processEchange(const int i1, const int i2);
        void processRotation(const int i, State::SensRotation sens);

        void configureScreen(int posX, int posY, int w, int h);
        VODZO::Vector2 getVectorFromPos(const int pos);

    private:
        int m_nMoves;
        int m_taille;
        VODZO::Vector2 m_vectOrig;

        virtual void _init() {};
};


class BoardSolo : public Board {
    public:
        BoardSolo() : Board(), nextObjectif(-1), timeElapsed(0.0) {};

        void update();

    private:
        void processRandom();
        float nextObjectif, timeElapsed;

        virtual void _init();

};


#endif
