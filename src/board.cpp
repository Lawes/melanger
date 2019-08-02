#include <iostream>
#include <algorithm>

#include "board.h"
#include "libwam/random.h"

#include "globals.h"

using namespace std;

Board::Board():
    m_nsx(1),
    m_nsy(1),
    m_taille(10),
    m_nMoves(0)
{
    
}

Board::~Board() {
    clear();
}

void Board::clear() {
    m_shapes.clear();
    m_lastmove.clear();
    m_actions.clear();
}

sf::Vector2f Board::getVectorFromPos(int pos) const {
        int iCol = pos % m_nsx;
        int iRow = (pos - iCol)/m_nsx;
        return sf::Vector2f(m_taille*iCol, m_taille*iRow) + m_origine;
}

void Board::start() {
    m_actions.start();
}

void Board::stop() {
    m_actions.finish();
}

void Board::update(float dt) {
    m_actions.update(dt);
    for(auto &shape: m_shapes)
        shape.update();
}

void Board::init(const string& imgname, const sf::FloatRect& box, int nsx, int nsy) {
    m_nMoves = 0;
    m_nsx = nsx;
    m_nsy = nsy;

    RM.get(imgname, m_bgtexture);
    auto s = m_bgtexture->getSize();

    float textsize = min(
        static_cast<float>(s.x)/static_cast<float>(m_nsx),
        static_cast<float>(s.y)/static_cast<float>(m_nsy)
    );

    float plotsize = min(
        static_cast<float>(box.width)/static_cast<float>(m_nsx),
        static_cast<float>(box.height)/static_cast<float>(m_nsy)
    );

    m_origine = sf::Vector2f(
        box.left + (box.width - plotsize*m_nsx + plotsize)/2,
        box.top  + (box.height - plotsize*m_nsy + plotsize)/2
    );

    m_taille = plotsize;

    m_shapes.clear();
    m_shapes.resize(nsx*nsy);
    m_lastmove.clear();

    size_t id=0;
    for(int iy=0; iy<nsy; ++iy) {
        for(int ix=0; ix<nsx; ++ix, ++id) {
            m_lastmove.push_back(id);
            BoardShape& shape = m_shapes[id];
            shape.setTextcoords(sf::FloatRect(textsize*ix, textsize*iy, textsize, textsize));
            shape.setTexture(m_bgtexture, RM.getTexture("cadre"));
        }
    }

    for(size_t i=0; i<m_shapes.size(); ++i)
        m_shapes[i].init(i+1, i, 0, getVectorFromPos(i), 0.47*m_taille);

}

void Board::doShuffle() {
    vector<size_t> indice(m_shapes.size());
    for(size_t i=0; i<m_shapes.size(); ++i)
        indice[i] = i;
    random_shuffle(indice.begin(), indice.end());
    for(size_t i=0; i<m_shapes.size(); ++i) {
        processEchange(i, indice[i], true);
        if( Random::Percent() > 0.5f) 
            for(int n=0; n<Random::Int(0,3); ++n)
                processRotation(i, SensRotation::Plus, true);
        else
            for(int n=0; n<Random::Int(0,3); ++n)
                processRotation(i, SensRotation::Moins, true);
    }
}

int Board::findshape(float x, float y) const {
    sf::Vector2f v(x,y);
    for(auto it = m_lastmove.rbegin(); it !=m_lastmove.rend(); ++it) {
        if( m_shapes[*it].in(v) )
            return *it;
    }
    return -1;
}

BoardShape& Board::getShape(int i) {
    return m_shapes[i];
}

void Board::lastMove(int id) {
    m_lastmove.remove(id);
    m_lastmove.push_back(id);
}

bool Board::isFinished() const {
    for(auto &s:m_shapes)
        if( !s.good() ) return false;
    return true;
}


void Board::processEchange(int i1, int i2, bool random) {
    if(!random) m_nMoves++;

    size_t ipos1 = m_shapes[i1].getState().ipos;
    size_t ipos2 = m_shapes[i2].getState().ipos;
    auto vpos1 = getVectorFromPos(ipos1);
    auto vpos2 = getVectorFromPos(ipos2);

    m_shapes[i1].moveTo(ipos2);
    m_actions.To(
        m_shapes[i1].getState().id,
        m_shapes[i1].getTranslate(),
        vpos2,
        2.0,
        [this,i1](){m_shapes[i1].endMove();});

    m_shapes[i2].moveTo(ipos1);
    m_actions.To(
        m_shapes[i2].getState().id,
        m_shapes[i2].getTranslate(),
        vpos1,
        2.0,
        [this,i2](){m_shapes[i2].endMove();});

    lastMove(i1);
    lastMove(i2);
	//Mixer::play("s");

}

void Board::processRotation(int indice, Board::SensRotation sens, bool random) {
    if(!random) m_nMoves++;

    if( sens == Board::SensRotation::Plus) 
        m_shapes[indice].rotatePlus();
    else
        m_shapes[indice].rotateMinus();
    
    //cout << "rotation : " << *(m_shapes[indice].getRotate()) << "-> " << m_shapes[indice].getRotationDeg() << endl;

    m_actions.To(
        -m_shapes[indice].getState().id,
        m_shapes[indice].getRotate(),
        m_shapes[indice].getRotationDeg(),
        1.0,
        [this,indice](){m_shapes[indice].endRotation();});

    lastMove(indice);
	//Mixer::play("s");
}

void Board::processRandomMove(bool keepgood) {
    vector<int> v;
    for(auto &s:m_shapes) {
        if(!(keepgood && s.good()) && s.isFixed())
            v.push_back(s.getState().id);
    }

    random_shuffle(v.begin(), v.end());
    if( Random::Percent() > 0.5) {
        if( v.size() < 2)
            return;

        int ipos1 = v[0]-1,
            ipos2 = v[1]-1;
        processEchange(ipos1, ipos2, true);
    }
    else {
        if(v.size() > 0) {
            int ipos = v[0]-1;
            if( Random::Percent() > 0.5)
                processRotation(ipos, Board::SensRotation::Plus, true);
            else
                processRotation(ipos, Board::SensRotation::Moins, true);
        }
    }

}

void Board::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    for(const auto &indice: m_lastmove)
        target.draw(m_shapes[indice], states);
}