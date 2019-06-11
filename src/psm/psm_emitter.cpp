#include <cmath>
#include <iostream>
#include "psm_vector.h"
#include "psm_emitter.h"
#include "psm_particule.h"

#include "random.h"

using namespace PSM;

Emitter::Emitter() :
    refVie(0.0f), refFriction(1.0f), refTaille(1.0f), refVitAngul(0.0f),
    stdVie(0.0f), stdCol(0.0f), stdTaille(0.0f), stdVit(0.0f), paused(false)
    {    }

void Emitter::operator() (Particule& p) {
    float r1 = Random::Percent();
    float r2 = Random::Percent();
    float r3 = Random::Percent();
    float r4 = Random::Percent();

    p.taille = refTaille*(1.0 + r2 * stdTaille);
    p.friction = refFriction*0.3*(2*r1+1);
    p.vitAngul = refVitAngul*((r1+r2+r3+r4)/4. - 1);
    p.color = refColor;
    if( stdCol > 0.0f) {
        p.color += stdCol*Color(r2-0.5f, r3-0.5f, r4-0.5f, r1);
    }
    p.angle = r1*360.0f;

    p.pos = refPos;
    p.oldPos = refPos;
    p.vel = refVit*(.5f+ stdVit*r1);

    p.vie = refVie*(1.0 + r4 * stdVie);

	_newParticule(p);

}

void HoleEmitter::_newParticule(Particule& p) {
	Vector r = randomVector();

	float proj = dotProduct(p.vel, r);
	Vector pdisq =  r - proj*p.vel;

	p.vel = m_mg*(p.vel + stdDir * r);
	//p.vel = m_mg*(std::abs(proj)*p.vel+0.1f*pdisq);
	p.pos += m_r*pdisq;
}

void LineEmitter::_newParticule(Particule& p) {
    float r = Random::Percent();
    p.pos = m_a + r * (m_b-m_a);
}


void SphereEmitter::_newParticule(Particule& p) {
    p.pos = randomVector()*m_rayon + refPos;
}

void ZoneEmitter::_newParticule(Particule& p) {
    p.pos.x = Random::Float(m_xmin, m_xmax);
    p.pos.y = Random::Float(m_ymin, m_ymax);
}

void Spray::_newParticule(Particule& p)
{
    Vector rand = randomVector();
    p.vel = refVit + stdDir * rand;
}

void Explo::_newParticule(Particule& p)
{
    p.vel = randomVector();
    p.vel *= m_mg*p.friction;
}