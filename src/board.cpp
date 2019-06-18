#include <iostream>
#include <algorithm>

#include "board.h"

#include "globals.h"

using namespace std;

Board::Board():
    m_nsx(1),
    m_nsy(1),
    m_nMoves(0),
    m_taille(10) 
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

    sf::Image *img;
    RM.get(imgname, img);
    m_bgtexture.loadFromImage(*img);
    auto s = img->getSize();

    float textsize = min(
        static_cast<float>(s.x)/static_cast<float>(m_nsx),
        static_cast<float>(s.y)/static_cast<float>(m_nsy)
    );

    float plotsize = min(
        static_cast<float>(box.width)/static_cast<float>(m_nsx),
        static_cast<float>(box.height)/static_cast<float>(m_nsy)
    );

    float totalsize = max(plotsize*m_nsx, plotsize*m_nsy);


    m_origine = sf::Vector2f(
        box.left + (box.width - totalsize + plotsize)/2,
        box.top  + (box.height - totalsize + plotsize)/2
    );

    m_taille = plotsize;

    m_shapes.clear();
    m_shapes.resize(nsx*nsy);
    m_lastmove.clear();
    vector<size_t> indice(m_shapes.size());

    size_t id=0;
    for(int iy=0; iy<nsy; ++iy) {
        for(int ix=0; ix<nsx; ++ix, ++id) {
            indice[id] = id;
            m_lastmove.push_back(id);
            BoardShape& shape = m_shapes[id];
            shape.setTextcoords(sf::FloatRect(textsize*ix, textsize*iy, textsize, textsize));
            shape.setTexture(&m_bgtexture, RM.getTexture("cadre"));
        }
    }

    random_shuffle(indice.begin(), indice.end());

    for(size_t i=0; i<m_shapes.size(); ++i)
        m_shapes[i].init(i, indice[i], 0, getVectorFromPos(indice[i]), 0.4*m_taille);

        //states[i].init(i, indice[i], Random::Int(-2, 1),
        //                getVectorFromPos(indice[i]), m_taille*(0.5-3./43));

}
/*
int findShape(vector<Shape>& liste, list<int>& ordre, int x, int y) {
    list<int>::const_iterator it = ordre.begin();
    int res = -1;
    Vector2 v(x,y);
    for(; it != ordre.end(); ++it) {
        if( liste[*it].in(v) )
            res = *it;
    }

    return res;
}*/

void Board::lastMove(size_t id) {
    m_lastmove.remove(id);
    m_lastmove.push_back(id);
}

bool Board::isFinished() const {
    for(auto &s:m_shapes)
        if( !s.good() ) return false;
    return true;
}

/* 
void Board::processEchange(const int i1, const int i2) {
    m_nMoves++;

    int ipos1 = states[i1].getPos();
    int ipos2 = states[i2].getPos();
    Vector2 pos1 = getVectorFromPos(ipos1);
    Vector2 pos2 = getVectorFromPos(ipos2);

    Functor_v fun1(&states[i1], &State::finDeplacement);

    states[i1].goPos(ipos2);
    tween.To(listeShape[i1].id(Shape::TRANSLATION),
             listeShape[i1].getTranslate(),
             pos2,
             2.0,
             fun1);

    Functor_v fun2(&states[i2], &State::finDeplacement);
    states[i2].goPos(ipos1);
    tween.To(listeShape[i2].id(Shape::TRANSLATION),
             listeShape[i2].getTranslate(),
             pos1,
             2.0,
             fun2);

    lastMove(i1);
    lastMove(i2);
	Mixer::play("s");

}

void Board::processRotation(const int indice, State::SensRotation sens) {
    m_nMoves++;

    Functor_v fun1(&states[indice], &State::finRotation);
    states[indice].goRotation(sens);
    lastMove(indice);
    tween.To(listeShape[indice].id(Shape::ROTATION),
             listeShape[indice].getRotate(),
             static_cast<float>(states[indice].getRotation()),
             1.0,
             fun1);

	Mixer::play("s");

}

void Board::handle_events(CInput& in) {

	if( select != select1 && select != -1)
		listeShape[select].set_select(false);

	select = findShape(listeShape, ordre, in.MouseX(), in.MouseY());
	if( select != -1) {
		listeShape[select].set_select(true);

	}

    if( in.MouseButton(SDL_BUTTON_LEFT) ) {
		if( pressed ) return;
		pressed = true;
		if( select == -1)
			return;

        if( select1 == -1) {
			select1 = select;
		}
        else if( select2 == -1 && select1 == select) {
			listeShape[select1].set_select(false);
            select1 = -1;

        }
        else if( select2 == -1 && select1 != select) {
            select2 = select;
            cout << "select (" << select1 << ',' << select2 << ')'<< endl;

            processEchange(select1, select2);

			listeShape[select1].set_select(false);
			listeShape[select2].set_select(false);
            select1 = -1;
            select2 = -1;

        }

    }
    else {
        pressed = false;
    }


	if( in.Key(SDLK_a) ) {
		if( select == -1)
			return;

		processRotation(select, State::ROT_MOINS);
		in.unKey(SDLK_a);
    }
    if( in.Key(SDLK_z) ) {
		if( select == -1)
			return;

		processRotation(select, State::ROT_PLUS);
		in.unKey(SDLK_z);
    }
}
*/

void Board::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    for(const auto &indice: m_lastmove)
        target.draw(m_shapes[indice], states);
}