#include <iostream>
#include <string>
#include <algorithm>
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include "board.h"
#include "Vector2.h"
#include "shape.h"
#include "Ressources.h"
#include "global.h"
#include "Random.h"
#include "state.h"
#include "Input.h"
#include "tweenId.h"
#include "timer.h"
#include "Box.h"
#include "sound.h"

#include "texture_utils.h"

using namespace std;
using namespace VODZO;
using gui::Box;


Board::~Board() {
    clear();
    cout << "clear Board" << endl;
}

Vector2 Board::getVectorFromPos(const int pos) {
        int iCol = pos % ImageConfig::nWidth;
        int iRow = (pos - iCol)/ImageConfig::nWidth;
        return m_taille*Vector2(iCol, iRow) + m_vectOrig;
//        return Vector2(iCol * 90 + 45, iRow*90 + 45+50);
}



void Board::init(const string& fileName, const Box& box) {
    m_nMoves = 0;
	select = 0;

    unsigned int id = 0;

    SDL_Surface *img = LoadIMG(fileName, GL_RGB);
//    cout << static_cast<unsigned int>(img->format->BitsPerPixel) << endl;
    SDL_Surface *dest = SDL_CreateRGBSurface(SDL_HWSURFACE, ImageConfig::sizeTexture, ImageConfig::sizeTexture, 24, 0, 0, 0, 0);
    SDL_Rect clip;
    clip.w = ImageConfig::sizeTexture;
    clip.h = ImageConfig::sizeTexture;
    Shape::setIdCadre(RMa.getIdTexture("cadre"));

    m_taille = min(static_cast<float>(box.h)/ImageConfig::nHeight, static_cast<float>(box.w)/ImageConfig::nWidth);


    m_vectOrig = (Vector2(box.w, box.h) -
                    m_taille*Vector2(ImageConfig::nWidth, ImageConfig::nHeight))/2
                    + Vector2(box.x+m_taille/2, box.y+m_taille/2);
    for(int ih=0; ih< ImageConfig::nHeight; ++ih)
    for(int iw=0; iw< ImageConfig::nWidth; ++iw) {
        clip.x = iw*ImageConfig::sizeTexture;
        clip.y = ih*ImageConfig::sizeTexture;

        SDL_BlitSurface(img, &clip, dest, NULL);
        genGlTexture(id, dest, GL_RGB);
        listeShape.push_back(Shape(id));
        states.push_back(State());

    }

    SDL_FreeSurface(img);
    SDL_FreeSurface(dest);

    vector<int> indice(states.size());
    for(size_t i=0; i<indice.size(); ++i) {
        ordre.push_back(static_cast<int>(i));
        indice[i] = i;
    }

    random_shuffle(indice.begin(), indice.end());


    for(size_t i=0; i<listeShape.size(); ++i) {
        states[i].setShape(&listeShape[i]);
        states[i].init(i, indice[i], Random::Int(-2, 1),
                        getVectorFromPos(indice[i]), m_taille*(0.5-3./43));

    }

    _init();

}

int findShape(vector<Shape>& liste, list<int>& ordre, int x, int y) {
    list<int>::const_iterator it = ordre.begin();
    int res = -1;
    Vector2 v(x,y);
    for(; it != ordre.end(); ++it) {
        if( liste[*it].in(v) )
            res = *it;
    }

    return res;
}

bool Board::finish() {
    for(size_t i=0; i<states.size(); ++i)
        if( !states[i].good() )
            return false;
    return true;

}

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

void Board::update() {

    tween.Update();
    for(size_t i=0; i<listeShape.size(); ++i) {
        if( states[i].good() ) {

            listeShape[i].setColor(Color::green);
        }

    }




}

void Board::draw() {
    list<int>::iterator it = ordre.begin();

   for( ; it != ordre.end(); ++it) {
        listeShape[*it].apply();
        listeShape[*it].draw();
   }

}

void Board::clear() {
    for(size_t i=0; i<listeShape.size(); ++i) {
        GLuint id = listeShape[i].getIdTexture();
        glDeleteTextures(1, &id);
    }
    listeShape.clear();

}

void Board::lastMove(const int id) {
    ordre.remove(id);
    ordre.push_back(id);

}

void BoardSolo::_init() {
    nextObjectif = -1.0;
    timeElapsed = 0.0;
}


void BoardSolo::processRandom() {
    int i1, i2;

    float r = Random::Percent();
    if( r < 0.5) {
        i1 = Random::Int(0, listeShape.size());
        i2 = Random::Int(0, listeShape.size());
        processEchange(i1, i2);
    }
    else {
        i1 = Random::Int(0, listeShape.size());
        if( r > 0.75)
            processRotation(i1, State::ROT_PLUS);
        else
            processRotation(i1, State::ROT_MOINS);

    }

}

void BoardSolo::update() {
    if( nextObjectif < 0 ) {
        cout << "boardSolo gogo " <<timeElapsed << endl;
        nextObjectif = 0.1;
    }

    timeElapsed += deltaTime;

    if( timeElapsed >= nextObjectif ) {
        nextObjectif = Random::Float(0.1, 1);
        timeElapsed = 0.0;
        processRandom();
        processRandom();
        processRandom();
    }

    Board::update();

}
