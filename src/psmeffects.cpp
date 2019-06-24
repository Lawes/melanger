#include "psmeffects.h"
#include "globals.h"

#include "random.h"

#include <vector>

using namespace std;

PSM::Color createRandomColor() {
    return PSM::Color(Random::Percent(),
                 Random::Percent(),
                 Random::Percent(),
                 1.0);
}

ZoneObscure::ZoneObscure(sf::FloatRect rect) :
    m_psm(100, 3.0f, 10.0f),
    m_psmline(2000, 50, 4)
{
    auto center = PSM::Vector(rect.left+rect.width/2, rect.top+rect.height/2);
    m_psm.getEmitter().setPos(center);
    m_psm.getEmitter().setVit(PSM::Vector(0, 200));
    m_psm.getEmitter().setTaille(200, 0.5);
    m_psm.getEmitter().setColor(PSM::Color::White*0.2, 0.0);

    m_psm.getMover().setCenter(center);
    m_psm.getMover().setAttraction(6.0);

    m_psm.getRender().setTexture(*RM.getTexture("fire1"));

	m_psm.getFormer().getFade().setBornes(0.0, 0.0);
	m_psm.getFormer().getFade().addPt(0.1, 0.4);
	m_psm.getFormer().getFade().addPt(0.5, 0.2);
	m_psm.getFormer().getFade().addPt(0.9, 0.4);

    m_psmline.getEmitter().setStdVie(0.5);
    m_psmline.getEmitter().setPos(center);
    m_psmline.getEmitter().setVit(PSM::Vector(0,1));
    m_psmline.getEmitter().setTaille(0.3);
    m_psmline.getEmitter().setColor(PSM::Color::Yellow);

    m_psmline.getMover().setCenter(center);
    m_psmline.getMover().setAttraction(-60.0);

    m_psmline.getRender().setLineWidth(3.0);

    m_psmline.getFormer().getFade().setBornes(0.0, 0.0);
    m_psmline.getFormer().getFade().addPt(0.6, 0.3);
    m_psmline.getFormer().getColor().setBornes(PSM::Color::Yellow, PSM::Color::White);
}

void ZoneObscure::start() {
    m_psmline.reset();
    m_psm.reset();
    m_psmline.start();
    m_psm.start();

}

void ZoneObscure::stop() {
    m_psm.stop();
    m_psmline.stop();
}

void ZoneObscure::update(float dt) {
    m_psm.update(dt);
    m_psmline.update(dt);
}

void ZoneObscure::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(m_psmline, states);
    target.draw(m_psm, states);
}

FeuxArtifice::FeuxArtifice(sf::FloatRect rec) :
    m_fumefuse(1000, 0.0f, 0.5f),
    m_fumepart(10000, 0.0f, 0.5f),
    m_fuse(400, 4.0f, 2.0f, 0.02f),
    m_part(2400, 0.0f, 1.1f, 0.01f),
    m_explosounds{"f1", "f2", "f3"}
{ 
    m_fumefuse.getFormer().getFade().setBornes(1.0f, 0.0f);
    m_fumefuse.getFormer().getFade().addPt(0.5f, 1.0f);
    m_fumefuse.getFormer().getColor().setBornes(PSM::Color::White*0.4, PSM::Color::White*0.1);
    m_fumefuse.getFormer().getSize().setBornes(0.0f, 6.0f);
    m_fumefuse.getFormer().getSize().addPt(0.5f, 4.0f);

    m_fumepart.getFormer().getFade().setBornes(1.0f, 0.0f);
    m_fumepart.getFormer().getFade().addPt(0.5f, 1.0f);
    m_fumepart.getFormer().getColor().setBornes(PSM::Color::Yellow*0.3, PSM::Color::White*0.2);
    m_fumepart.getFormer().getSize().setBornes(0.0f, 5.0f);
    m_fumepart.getFormer().getSize().addPt(0.5f, 1.5f);

    m_fuse.getEmitter().setStdDir(80.0f);
    m_fuse.getEmitter().setColor(PSM::Color::Purple, 0.5f);
    m_fuse.getEmitter().setPhysic(0.5, 0.1);
    m_fuse.getEmitter().setTaille(20.0f);
    //m_fuse.getRender().setSize(5.0f);
    m_fuse.getMover().setGravity(PSM::Vector(0.0f, 30.0f));
    m_fuse.attachFunc([this](PSM::Vector v){ m_fumefuse.createParticule(v);});
    m_fuse.endPartFunc([this](PSM::Vector v){paf(v);});

    m_part.getEmitter().setStdVie(1.0);
    m_part.getEmitter().setVit(PSM::Vector(0.0f,100.0f), 1.0);
    m_part.getEmitter().setColor(PSM::Color::Red);
    m_part.getEmitter().setTaille(12.0f);
    m_part.getEmitter().setPhysic(0.8, 3.0);
    m_part.getMover().setGravity(PSM::Vector(0.0f, 30.0f));
    m_part.attachFunc([this](PSM::Vector v) { m_fumepart.createParticule(v);});
    m_part.getFormer().getFade().setBornes(1.0, 0.0);
    m_part.getFormer().getFade().addPt(.8, 1.0);

    m_part.getRender().setTexture(*RM.getTexture("sun"));
    m_fumefuse.getRender().setTexture(*RM.getTexture("fire1"));
    m_fumepart.getRender().setTexture(*RM.getTexture("fire1"));
    m_fuse.getRender().setTexture(*RM.getTexture("fuse"));

    m_fuse.getEmitter().setVit(PSM::Vector(0, -250));
    m_fuse.getEmitter().setPos(PSM::Vector(275, 700));
}

void FeuxArtifice::paf(PSM::Vector v) {
    //MIXER.play(*(Random::randIterator(m_explosounds.begin(),3)), Random::Percent()*30.0f);
    m_part.getEmitter().setColor(createRandomColor(), 0.3);
    m_part.getEmitter().setPos(v);
    m_part.addParticules(40);
}

void FeuxArtifice::start() {
    m_fuse.reset();
    m_part.reset();
    m_fumefuse.reset();
    m_fumepart.reset();
    m_fuse.start();
}

void FeuxArtifice::stop() {
    m_fuse.stop();
}

void FeuxArtifice::update(float dt) {
    m_fumefuse.update(dt);
    m_fumepart.update(dt);
    m_fuse.update(dt);
    m_part.update(dt);
}

void FeuxArtifice::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(m_fumefuse, states);
    target.draw(m_fumepart, states);
    target.draw(m_fuse, states);
    target.draw(m_part, states);
}