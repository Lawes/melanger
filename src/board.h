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

        void doShuffle();

        void processEchange(int i1, int i2, bool random=false);
        void processRotation(int i, SensRotation sens, bool random=false);
        void processRandomMove(bool keepgood=true);

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
        sf::Texture *m_bgtexture;

    DERIVED_GameObject

};

#endif
