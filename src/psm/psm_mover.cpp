
#include <cmath>
#include <iostream>
#include "psm_mover.h"
#include "psm_type.h"
#include "psm_particule.h"

using namespace PSM;

bool Mover::updateParticule(Particule &p, const float dt) {
	p.vie -= dt;
	return p.vie < 0.0f;
}

bool Espace::updateParticule(Particule& p, const float dt) {
    p.oldPos = p.pos;
	p.pos += p.vel*dt;
    p.angle += p.vitAngul*dt;

	p.vie -= dt;
	//std::cout << "mover " << p.pos.x << " " << p.pos.y << " : " << p.vie << std::endl;
	return p.vie < 0.0f;
}

bool Earth::updateParticule(Particule& p, const float dt) {
    p.oldPos = p.pos;
	p.vel += gravity*dt;// - p.friction* (p.vel-wind);
    p.pos += p.vel*dt;

    p.angle += p.vitAngul*dt;

	p.vie -= dt;
	return p.vie < 0.0f;
}

bool Vortex::updateParticule(Particule& p, const float dt) {
	Vector t = p.pos - center;

	float d = std::max(length(t), 0.1f);

    p.oldPos = p.pos;

	p.vel += m_attraction*(-t/d - 0.01f*p.vel)*dt;
	p.pos += p.vel*dt;

    p.angle += p.vitAngul*dt;

	p.vie -= dt;
	return p.vie < 0.0f;

}
