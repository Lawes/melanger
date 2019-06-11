#include "shape.h"
#include "Vector2.h"
#include "Color.h"
#include "global.h"

#include <cmath>

#include <SDL/SDL_opengl.h>

using namespace VODZO;

int Shape::currentId = 0;
unsigned int Shape::idCadre_ = 0;

int
SIAM( const Vector2& p0, const Vector2& p1, const Vector2& p2) 
{
	float dx1, dx2, dy1, dy2;

	dx1=p1.x-p0.x; dy1=p1.y-p0.y;
	dx2=p2.x-p0.x; dy2=p2.y-p0.y;

	if( dx1*dy2 > dy1*dx2 ) return 1;
	if( dx1*dy2 < dy1*dx2 ) return -1;
	if( (dx1*dx2 < 0.0) || (dy1*dy2 < 0.0) ) return -1;
	if( (dx1*dx1+dy1*dy1) < (dx2*dx2+dy2*dy2) ) return 1;

	return 0;
}


void doRotation(Vector2& pRot,  const float cs, const float sn, const Vector2& p) {
    pRot.x = cs * p.x - sn * p.y;
    pRot.y = sn * p.x + cs * p.y;

}


void Rect::doTransformation(const float angle, 
    const float scale, const Vector2& trans) {

    float cs = cos(angle*deg2Rad);
    float sn = sin(angle*deg2Rad);

    doRotation(p1, cs, sn, Vector2(1,1));
    p1 *= scale;
    doRotation(p2, cs, sn, Vector2(-1,1));
    p2 *= scale;
//    doRotation(p3, cs, sn, Vector2(-1,-1));
//    p3 *= scale;
//    doRotation(p4, cs, sn, Vector2(1,-1));
//    p4 *= scale;

    p3 = -p1;
    p4 = -p2;
    
    p1 += trans;
    p2 += trans;
    p3 += trans;
    p4 += trans;


}

bool Rect::in(const Vector2& p) {
    return (SIAM(p1, p2, p) >=0 && SIAM(p2, p3, p) >= 0 &&
       SIAM(p3, p4, p) >= 0 && SIAM(p4, p1, p) >= 0);
}


Shape::Shape(): id_(0), color_(Color::black), idTexture_(0), translate_(0,0), theta_(0),  scale_(1.0), selected_(false)  {
    id_ = ++currentId;
    
}

Shape::Shape(const unsigned int id): id_(0), color_(Color::black), idTexture_(id), translate_(0,0), theta_(0),  scale_(1.0), selected_(false) {
    id_ = ++currentId;
    
}

void Shape::setTransform(const Vector2& t, const float angle, const float scale=1.0) {
    translate_ = t;
    theta_ = angle;
    scale_ = scale;
}


void Shape::apply() {
    m_pt.doTransformation(theta_, scale_, translate_);
    m_cadre.doTransformation(theta_, scale_ + 6 *scale_/43, translate_);

}

bool Shape::in( const Vector2& p) {
    return m_pt.in(p);
}

void Shape::draw() {

    glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, idTexture_);
    glColor3d(1,1,1);
	glBegin(GL_QUADS);
		glTexCoord2d(1, 1); glVertex2fv(&m_pt.p1.x);
		glTexCoord2d(0, 1); glVertex2fv(&m_pt.p2.x);
		glTexCoord2d(0, 0); glVertex2fv(&m_pt.p3.x);
		glTexCoord2d(1, 0); glVertex2fv(&m_pt.p4.x);
	glEnd();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBindTexture(GL_TEXTURE_2D, idCadre_);

	glColor3fv(&color_.r);
    
	glBegin(GL_QUADS);
		glTexCoord2d(1, 1); glVertex2fv(&m_cadre.p1.x);
		glTexCoord2d(0, 1); glVertex2fv(&m_cadre.p2.x);
		glTexCoord2d(0, 0); glVertex2fv(&m_cadre.p3.x);
		glTexCoord2d(1, 0); glVertex2fv(&m_cadre.p4.x);
	glEnd();

    glDisable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);

	if( selected_ ) {
		Color col = Color::blue;
		glLineWidth(3.0);
		glColor3fv(&col.r);
		glBegin(GL_LINE_LOOP);
		glVertex2fv(&m_pt.p1.x);
		glVertex2fv(&m_pt.p2.x);
		glVertex2fv(&m_pt.p3.x);
		glVertex2fv(&m_pt.p4.x);
		glEnd();
	}
}

