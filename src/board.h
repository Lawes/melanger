#ifndef BOARD_HEADER
#define BOARD_HEADER

#include <vector>
#include <string>
#include <list>

#include <SFML/Graphics.hpp>

#include "gameobject.h"
#include "tween.h"
#include "boardshape.h"


class Board : public GameObject {
    public:
        enum class SensRotation { Plus, Moins};
        Board();
        virtual ~Board();

        void init(const std::string& imgname, const sf::FloatRect& box, int nsx, int nsy);
        void clear();

        int getNbMoves() const { return m_nMoves;}
        bool isFinished() const;
        int findshape(float x, float y) const;
        BoardShape& getShape(int i);

        void processEchange(int i1, int i2);
        void processRotation(int i, SensRotation sens);

    private:
        void lastMove(int id);

    protected:
        std::vector<BoardShape> m_shapes;
        std::list<int> m_lastmove;
        TweenCollection m_actions;

        sf::Vector2f getVectorFromPos(int pos) const;

    private:
        std::size_t m_nsx, m_nsy;
        float m_taille;
        int m_nMoves;
        sf::Vector2f m_origine;
        sf::Texture m_bgtexture;

    DERIVED_GameObject

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
