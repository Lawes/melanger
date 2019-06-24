#include <iostream>
#include <cmath>
#include <SFML/OpenGL.hpp>

#include "boardshape.h"
#include "libwam/geometry.h"

using namespace std;

template<class V>
void doRotation(V& pRot,  float cs, float sn, const V& p) {
    pRot.x = cs * p.x - sn * p.y;
    pRot.y = sn * p.x + cs * p.y;
}

BoardShape::BoardShape():
	m_state{0,0,0, false, false, false, false},
	m_pt(sf::Quads, 4),
	m_cadre(sf::Quads, 4),
	m_select(sf::LineStrip, 5),
	m_translate(0,0),
	m_theta(0),
	m_scale(1.0),
	m_bord(nullptr),
	m_img(nullptr)
{
	setColor(sf::Color::Black);
}

bool BoardShape::good() const {
	return m_state.ipos == m_state.id
		&& m_state.irot == 0
		&& isFixed();
}

bool BoardShape::isFixed() const {
	return !m_state.isMoving && !m_state.isRotating;
}

void BoardShape::endMove() {
	m_state.isMoving = false;
	if( isFixed() ) setColor(sf::Color::Black);
}

void BoardShape::endRotation() {
	m_state.isRotating = false;
	m_state.irot = m_state.irot%4;
	m_theta = BoardShape::_getRotation(m_state.irot);
	//cout << "end rotation " << m_state.irot << ", deg: " << m_theta << endl;
	if( isFixed() ) setColor(sf::Color::Black);
}

void BoardShape::moveTo(int pos) {
	m_state.ipos = pos;
	m_state.isMoving = true;
	setColor(sf::Color::Red);
}

void BoardShape::rotatePlus() {
	m_state.irot += 1;
	m_state.isRotating = true;
	setColor(sf::Color::Red);
}

void BoardShape::rotateMinus() {
	m_state.irot -= 1;
	m_state.isRotating = true;
	setColor(sf::Color::Red);
}

void BoardShape::init(int id, int ipos, int irot, const sf::Vector2f& v, float size) {
	m_state = {id, ipos, irot, false, false, false, false};

	setTransform(v, BoardShape::_getRotation(irot), size);
	update();
}

void BoardShape::setTexture(sf::Texture *img, sf::Texture *bord) {
	m_bord = bord;
	m_img = img;

	auto size = m_bord->getSize();
	m_cadre[0].texCoords = sf::Vector2f(0,0);
	m_cadre[1].texCoords = sf::Vector2f(size.x,0);
	m_cadre[2].texCoords = sf::Vector2f(size.x,size.y);
	m_cadre[3].texCoords = sf::Vector2f(0,size.y);	
}

void BoardShape::setTextcoords(const sf::FloatRect& rect) {
	m_pt[0].texCoords = sf::Vector2f(rect.left, rect.top);
	m_pt[1].texCoords = sf::Vector2f(rect.left+rect.width, rect.top);
	m_pt[2].texCoords = sf::Vector2f(rect.left+rect.width, rect.top+rect.height);
	m_pt[3].texCoords = sf::Vector2f(rect.left, rect.top+rect.height);
}

void setColorInVA(sf::VertexArray &va, sf::Color col) {
	for(size_t i=0; i<va.getVertexCount(); ++i)
		va[i].color = col;
}

void BoardShape::setColor(const sf::Color& col) {
	setColorInVA(m_cadre, col);
}

void BoardShape::setTransform(const sf::Vector2f& t, float angle, float scale=1.0) {
    m_translate = t;
    m_theta = angle;
    m_scale = scale;
}

void BoardShape::update() {
	doTransformation(m_theta, m_scale, m_translate);
	if( good()) setColor(sf::Color::Green);
}

bool BoardShape::in( const sf::Vector2f& p) const {
	return ptInShape(
		p,
		m_pt[0].position, m_pt[1].position,
		m_pt[2].position, m_pt[3].position);
}

void BoardShape::doTransformation(
	float angle, 
	float scale,
	const sf::Vector2f& trans) 
{
    const float deg2Rad	= 0.01745329f;
	float scadre = scale + 6 *scale/43;
    float cs = std::cos(angle*deg2Rad);
    float sn = std::sin(angle*deg2Rad);

    doRotation(m_pt[0].position, cs, sn, sf::Vector2f(-1,-1));
	m_cadre[0].position = m_pt[0].position * scadre;
    m_pt[0].position *= scale;
    doRotation(m_pt[1].position, cs, sn, sf::Vector2f(1,-1));
	m_cadre[1].position = m_pt[1].position * scadre;	
    m_pt[1].position *= scale;

	m_cadre[2].position = -m_cadre[0].position + trans;
	m_cadre[3].position = -m_cadre[1].position + trans;
	m_cadre[0].position += trans;
	m_cadre[1].position += trans;

    m_pt[2].position = -m_pt[0].position + trans;
    m_pt[3].position = -m_pt[1].position + trans;    
    m_pt[0].position += trans;
    m_pt[1].position += trans;

	for(size_t i=0; i<4; ++i)
		m_select[i].position = m_pt[i].position;
	m_select[4].position = m_pt[0].position;
	setColorInVA(m_select, sf::Color::Blue);
	
}

void BoardShape::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	states.texture = m_img;
	target.draw(m_pt, states);

	if( isSelected() ) {
		states.texture = nullptr;
		sf::VertexArray vs(m_select);
		setColorInVA(vs, sf::Color::Yellow);
		glEnable(GL_LINE_SMOOTH);
		glLineWidth(10.0);
		target.draw(vs, states);
	}

	states.texture = m_bord;
	target.draw(m_cadre, states);

	if( isOverMouse()) {
		states.texture = nullptr;
		glEnable(GL_LINE_SMOOTH);
		glLineWidth(3.0);
		target.draw(m_select, states);
	}
}

