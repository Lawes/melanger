#include "anim.h"

#include <iostream>

using namespace std;

Anim::Anim() :iFrame(0), incFrame(1), tCur(0.0), 
	isEndQ(true), cycleQ(false)
{

}

Anim::~Anim() {
	//printf("Destructeur Anim\n");
}

void Anim::addFrame(sf::Texture *t, float dt) {
	m_listeFrames.push_back({t, dt});
}

void Anim::begin() {
	isEndQ=false;
	tCur=0.0;
	iFrame=0;
	incFrame=1;
}

void Anim::update(float dt) {
    //cout << dt << endl;
	if( isEndQ ) return;

	int nframes = m_listeFrames.size();
	tCur += dt;

	if(tCur > m_listeFrames[iFrame].dt ) {
		tCur-=m_listeFrames[iFrame].dt;

		iFrame += incFrame;

		if( iFrame == nframes ) {
			iFrame--;
			if( cycleQ ) {
				iFrame--;
				incFrame = -1;
			}
			else {
				isEndQ=true;
				incFrame=0;
			}
		}
		else if( iFrame<0 ) {
			iFrame=1;
			incFrame = 1;
		}
	}

}
void Anim::goNext(void) {
    ++iFrame;
	iFrame=iFrame%(m_listeFrames.size());
}

float Anim::getRatio() const {
    return tCur/m_listeFrames[iFrame].dt;
}


void Anim::print() const {
	VAnimFrame::const_iterator it = m_listeFrames.begin();
	cout << "print animation" << endl;
	for(; it != m_listeFrames.end(); ++it) {
		cout << "frame : " << it->texture << " " << it->dt << endl;
	}
}