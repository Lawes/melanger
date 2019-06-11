#include <SFML/OpenGL.hpp>
#include <iostream>

#include "psm_type.h"
#include "psm_render.h"
#include "psm_particule.h"
#include "trigocossin.h"

using namespace std;
using namespace PSM;

namespace {
	TrigoCosSin tableCosSin(2);

}

Render::Render(sf::PrimitiveType t, unsigned int n):
	m_blend(sf::BlendMode::SrcAlpha , sf::BlendMode::OneMinusSrcAlpha),
	m_texture(NULL),
	m_displayBuffer(t, n),
    m_maxParticuleNum(n),
	m_xtextsize(0),
	m_ytextsize(0)
	    { }

void Render::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	draw_();
	states.blendMode = m_blend;
	states.texture = m_texture;
	//std::cout << "texture " << states.texture << std::endl;
	target.draw(m_displayBuffer, states);
}

render_POINTS::render_POINTS(unsigned int n) : Render(sf::Points, n), m_pointSize(1.0f) 
{ }

void render_POINTS::draw_() const {
    glEnable( GL_POINT_SMOOTH );
    glPointSize(m_pointSize);
}

void render_POINTS::operator() (vector<Particule>::const_iterator p,
								const unsigned numToDraw) const {

	if( numToDraw > m_maxParticuleNum) {
		//std::cout << "pas bien" << std::endl;
		return;
	}
	m_displayBuffer.clear();

	for(unsigned int i=0; i<numToDraw; ++i, ++p) {
		sf::Vertex v;
		v.position = PSM::Vector(p->pos.x, p->pos.y);
		v.color = touint(p->color);
		//std::cout << int(v.color.r) << " " << int(v.color.g) << " " << int(v.color.b) << std::endl;
		//std::cout << "c " << (p->color.r) << " " << (p->color.g) << " " << (p->color.b) << " " << (p->color.a) << std::endl;
		//std::cout << "u " << v.position.x << " " << v.position.y << std::endl;
		m_displayBuffer.append(v);
	}

}

render_SPRITES::render_SPRITES(unsigned int n) :
	Render(sf::Quads, n)
{ }

void render_SPRITES::operator() (vector<Particule>::const_iterator p,
								 const unsigned numToDraw) const {
	if( numToDraw > m_maxParticuleNum) return;
	m_displayBuffer.clear();
    float cosA, sinA;
	sf::Vector2f m_up(0,-1), upQuad, sideQuad;

	for(unsigned int i=0; i<numToDraw; ++i, ++p) {
		cosA = tableCosSin.Cos(p->angle);
		sinA = tableCosSin.Sin(p->angle);

		upQuad.x = cosA * m_up.x + sinA * m_up.y;
		upQuad.y = -sinA * m_up.x + cosA * m_up.y;

		sideQuad.x = -upQuad.y,
		sideQuad.y = upQuad.x;

		upQuad *= p->taille;
		sideQuad *= p->taille;
		sf::Vertex v1, v2, v3, v4;
		sf::Color c = touint(p->color);
		v1.position = sf::Vector2f(p->pos.x + sideQuad.x + upQuad.x,  p->pos.y + sideQuad.y + upQuad.y);
		v1.texCoords = sf::Vector2f(m_xtextsize,m_ytextsize);
		v1.color = c;
		v2.position = sf::Vector2f(p->pos.x - sideQuad.x + upQuad.x,  p->pos.y - sideQuad.y + upQuad.y);
		v2.texCoords = sf::Vector2f(0,m_ytextsize);
		v2.color = c;
		v3.position = sf::Vector2f(p->pos.x - sideQuad.x - upQuad.x,  p->pos.y - sideQuad.y - upQuad.y);
		v3.texCoords = sf::Vector2f(0,0);
		v3.color =  c;
		v4.position = sf::Vector2f(p->pos.x + sideQuad.x - upQuad.x,  p->pos.y + sideQuad.y - upQuad.y);
		v4.texCoords = sf::Vector2f(m_xtextsize,0);
		v4.color =  c;
		m_displayBuffer.append(v3);
		m_displayBuffer.append(v2);
		m_displayBuffer.append(v1);
		m_displayBuffer.append(v4);
	}
}



render_LINES::render_LINES(unsigned int n) : Render(sf::Lines, n), m_lineWidth(1.0f) 
{ }
void render_LINES::draw_() const {
	glEnable(GL_LINE_SMOOTH);
	glLineWidth(m_lineWidth);
}

void render_LINES::operator() (vector<Particule>::const_iterator p,
								 const unsigned numToDraw) const {

	if( numToDraw > m_maxParticuleNum) return;
	m_displayBuffer.clear();

	for(unsigned int i=0; i<numToDraw; ++i, ++p) {
		sf::Vertex v, vend;
		v.position = sf::Vector2f(p->pos.x, p->pos.y);
		v.color = touint(p->color);
		m_displayBuffer.append(v);
		vend.position = sf::Vector2f(p->pos.x + p->vel.x * p->taille, p->pos.y + p->vel.y * p->taille);
		vend.color = touint(p->color);
		m_displayBuffer.append(vend);
	}
}