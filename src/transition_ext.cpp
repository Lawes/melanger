#include <SFML/OpenGL.hpp>

#include "transition_ext.h"
#include "libwam/random.h"

#include <iostream>
#include <cmath>
using namespace std;

void TestTansition::_begin() {

}

void TestTansition::_end() {

}

void TestTansition::update(float dt) {

}

void TestTansition::draw(sf::RenderTarget &win) const {
    sf::RectangleShape s1, s2;

    s1.setSize(sf::Vector2f(200,200));
    s2.setSize(sf::Vector2f(200,200));

    s1.setTexture(&m_from);
    s2.setTexture(&m_to);

    s1.setPosition(0,0);
    s2.setPosition(300,300);

    win.draw(s1);
    win.draw(s2);
}

void TestTansition::load() {}

void FonduTransition::load() {
    auto box = m_context->getBox();
    m_before.setSize(sf::Vector2f(box.width, box.height));
    m_before.setPosition(0,0);
    m_before.setFillColor(sf::Color::White);
    m_after.setSize(sf::Vector2f(box.width, box.height));
    m_after.setPosition(0,0);
    m_after.setFillColor(sf::Color::White);

}


void FonduTransition::_begin() {
    beginTransitionTime();
    m_before.setTexture(&m_from);
    m_after.setTexture(&m_to);
}

void FonduTransition::_end() {

}

void FonduTransition::update(float dt) {
    if( updateTransitionTime(dt)) {
        end();
        return;
    }
    m_after.setFillColor(sf::Color(255,255,255,m_p*255));
}

void FonduTransition::draw(sf::RenderTarget &win) const {
    win.draw(m_before);
    win.draw(m_after);
}

sf::Vector2f randomVector(float r) {
    float angle = Random::Percent()*360.0f;
    return sf::Vector2f( r*std::cos(angle), r*std::sin(angle));
}

void RainTransition::_begin() {
    beginTransitionTime();
    auto b = m_from.getSize();
    float dx = static_cast<float>(b.x)/m_sizex, dy = static_cast<float>(b.y)/m_sizey;

    auto box = m_context->getBox();
    float screendx = box.width/m_sizex, screendy = box.height/m_sizey;

    m_actions.clear();
    m_shapes1.clear();
    m_shapes2.clear();

    for(int iy=0; iy<m_sizey; ++iy)
    for(int ix=0; ix<m_sizex; ++ix) {
        float px = dx*ix+0.5f, py=dy*iy+0.5f;
        sf::IntRect rect(px, py, dx+0.5f, dy+0.5f);
        m_shapes1.add(rect, {box.left+ix*screendx+screendx/2,box.top+iy*screendy+screendy/2}, {screendx/2,screendy/2}, 0);
        m_shapes2.add(
            rect,
            sf::Vector2f(box.left+box.width/2, box.top+box.height/2)+randomVector( box.width),
            {screendx/2,screendy/2},
            Random::Float(-180,180));
    }
    int count=0;
    for(int iy=0; iy<m_sizey; ++iy)
    for(int ix=0; ix<m_sizex; ++ix, ++count) {
        auto &s1 = m_shapes1.get(count);
        m_actions.To(&(s1.pos), sf::Vector2f(box.left+box.width/2, box.top+box.height/2)+randomVector( box.width), m_totaltime);
        m_actions.To(&(s1.angle), Random::Float(-180,-180), m_totaltime);        
        auto &s2 = m_shapes2.get(count);
        m_actions.To(&(s2.pos), {box.left+ix*screendx+screendx/2,box.top+iy*screendy+screendy/2}, m_totaltime/2);
        m_actions.To(&(s2.angle), 0.0f, m_totaltime/2);
    }
    
    m_shapes1.setTexture(&m_from);
    m_shapes2.setTexture(&m_to);
    m_actions.start();

}

void RainTransition::_end() {
    m_actions.finish();
}

void RainTransition::update(float dt) {
    m_actions.update(dt);
    m_shapes2.update();
    m_shapes1.update();
    if( updateTransitionTime(dt))
        end();
}

void RainTransition::draw(sf::RenderTarget &win) const {
    m_shapes2.draw(win);
    m_shapes1.draw(win);
}

void RainTransition::load() {
    m_sizex=50;
    m_sizey=50;

}

void VerreTransition::_begin() {
    beginTransitionTime();
    auto b = m_from.getSize();
    float dx = static_cast<float>(b.x)/m_sizex, dy = static_cast<float>(b.y)/m_sizey;

    auto box = m_context->getBox();
    float screendx = box.width/m_sizex, screendy = box.height/m_sizey;

    m_actions.clear();
    m_shapes1.clear();
    m_shapes2.clear();

    for(int iy=0; iy<m_sizey; ++iy)
    for(int ix=0; ix<m_sizex; ++ix) {
        float px = dx*ix+0.5f, py=dy*iy+0.5f;
        sf::IntRect rect(px, py, dx+0.5f, dy+0.5f);
        m_shapes1.add(rect, {box.left+ix*screendx+screendx/2,box.top+iy*screendy+screendy/2}, {screendx/2,screendy/2}, 0, 255.0f);
        m_shapes2.add(rect, {box.left+ix*screendx+screendx/2,box.top+iy*screendy+screendy/2}, {screendx/2,screendy/2}, 0, 0.0f);
    }
    m_shapes1.setTexture(&m_from);
    m_shapes2.setTexture(&m_to);
}

void VerreTransition::_end() {
    m_actions.finish();
}

void VerreTransition::update(float dt) {
    m_actions.update(dt);
    m_shapes2.update();
    m_shapes1.update();
    if( updateTransitionTime(dt))
        end();
}

void VerreTransition::draw(sf::RenderTarget &win) const {
    m_shapes2.draw(win);
    m_shapes1.draw(win);
    win.draw(m_va);
}

void VerreTransition::load() {
    m_sizex=50;
    m_sizey=50;
    auto box = m_context->getBox();
    float screendx = box.width/m_sizex, screendy = box.height/m_sizey;

	glEnable(GL_LINE_SMOOTH);
	glLineWidth(2.0);
    for(int ix=0; ix<m_sizex; ix++) {
        sf::Vertex v1, v2;
        v1.position = sf::Vector2f(box.left+ix*screendx,box.top);
        v2.position = sf::Vector2f(box.left+ix*screendx,box.top+box.height);
        v1.color = sf::Color(0,0,0);
        v2.color = sf::Color(0,0,0);
        m_va.append(v1);
        m_va.append(v2);
    }
    for(int iy=0; iy<m_sizey; iy++) {
        sf::Vertex v1, v2;
        v1.position = sf::Vector2f(box.left, box.top+screendy*iy);
        v2.position = sf::Vector2f(box.left+box.width,box.top+screendy*iy);
        v1.color = sf::Color(0,0,0);
        v2.color = sf::Color(0,0,0);
        m_va.append(v1);
        m_va.append(v2);
    }
}


/*
void FonduTransition::_begin() { }
void FonduTransition::_end() { }
void FonduTransition::update(float dt) { }
void FonduTransition::draw(sf::RenderTarget &win) const { }
void FonduTransition::load() {}
*/

/*



void VerreTransition::_captureScreen(SDL_Surface *img1, SDL_Surface *img2) {


    SDL_Rect clip;
    clip.w = static_cast<int>(dxf);
    clip.h = static_cast<int>(dyf);

    SDL_Surface *dest = SDL_CreateRGBSurface(SDL_HWSURFACE,
                            clip.w, clip.h,
                            24, 0, 0, 0, 0);

    for(int ih=0; ih< SizeY; ++ih)
    for(int iw=0; iw< SizeX; ++iw) {
        SpriteContainer& tmp = m_listeSprite[ih*SizeX + iw];
        clip.x = iw*dxf;
        clip.y = ih*dyf;
        SDL_BlitSurface(img1, &clip, dest, NULL);
        tmp.sp[0].init(dest, GL_RGB);
        SDL_BlitSurface(img2, &clip, dest, NULL);
        tmp.sp[1].init(dest, GL_RGB);

        tmp.posX = clip.x;
        tmp.posY = clip.y;
        tmp.angleZ = 0.0;
        tmp.alpha = 0.0;


    }

    SDL_FreeSurface(dest);
}


void VerreTransition::_init() {

    load(m_context->getCurrent(), m_context->getNext());

    m_ordre.reserve(SizeX*SizeY);
    for(int i=0; i<SizeX*SizeY; ++i)
        m_ordre.push_back(i);

    random_shuffle(m_ordre.begin(), m_ordre.end());


}

void VerreTransition::draw() {
    std::vector< SpriteContainer >::iterator it = m_listeSprite.begin();
    for(; it != m_listeSprite.end(); ++it) {
        Color col = Color::white;
        col.a = 1.0 - it->alpha;
        glColor4fv(&col.r);
        it->sp[0].draw(it->posX, it->posY, dxf, dyf);
        col.a = it->alpha;
        glColor4fv(&col.r);
        it->sp[1].draw(it->posX, it->posY, dxf, dyf);
    }

    glColor3d(0,0,0);
	glLineWidth(2.0);
    glBegin(GL_LINES);
    for(int ix=0; ix<SizeX; ix++) {
        glVertex2f(ix*dxf, 0.0);
        glVertex2f(ix*dxf, Windows_height);
    }
    for(int iy=0; iy<SizeY; iy++) {
        glVertex2f(0.0, iy*dyf);
        glVertex2f(Windows_width, iy*dyf);
    }
    glEnd();

}

void VerreTransition::update() {
    Transition::update();
    float time = m_timeLimit * (1.0-m_p);
    int toLaunch = SizeX*SizeY * (-1.0 + m_p/0.8)
                    + m_ordre.size();

    for( int i=0; i<toLaunch; ++i) {
        if( m_ordre.empty()) break;
        int iSprite = m_ordre.back();
        m_ordre.pop_back();
        m_listeSprite[iSprite].alpha = 0.0;
        m_mixer.add( new TweenerLin<float>(
                &(m_listeSprite[iSprite].alpha),
                1.0, time));

    }

    m_mixer.update();
}

const int RainTransition::SizeX = 50;
const int RainTransition::SizeY = 35;



void RainTransition::_captureScreen(SDL_Surface *img1){

    SDL_Rect clip;
    clip.w = static_cast<int>(dxf);
    clip.h = static_cast<int>(dyf);

    SDL_Surface *dest = SDL_CreateRGBSurface(SDL_HWSURFACE,
                            clip.w, clip.h,
                            24, 0, 0, 0, 0);

    for(int ih=0; ih< SizeY; ++ih) {

            for(int iw=0; iw< SizeX; ++iw) {
                SpriteContainer& tmp = m_listeSprite[ih*SizeX + iw];
                clip.x = static_cast<int>(dxf*iw);
                clip.y = static_cast<int>(dyf*ih);
                SDL_BlitSurface(img1, &clip, dest, NULL);
                tmp.sp.init(dest, GL_RGB);
                tmp.pos = Vector2(clip.x, clip.y);
                tmp.angleZ = 0.0;
                tmp.alpha = 0.0;

        }
    }


    SDL_FreeSurface(dest);

}


void RainTransition::_init() {
    load(m_context->getCurrent(), m_context->getNext());
    m_ordre.reserve(SizeX*SizeY);
    for(int i=0; i<SizeX*SizeY; ++i)
        m_ordre.push_back(i);

    random_shuffle(m_ordre.begin(), m_ordre.end());

    to_plot.reserve(SizeX*SizeY);

}


void RainTransition::draw(){
    glColor4f(1.0, 1.0, 1.0, 1.0);
    sp_bg.draw(0,0,
        Windows_width,
        Windows_height);

    glColor3d(1,1,1);

    vector<int>::iterator it = to_plot.begin();
    for(; it != to_plot.end(); ++it)
        m_listeSprite[*it].sp.drawR(m_listeSprite[*it].pos.x + dxf/2, m_listeSprite[*it].pos.y + dyf/2, dxf+2, dyf+2, m_listeSprite[*it].angleZ);

}

void RainTransition::update() {
    static const Vector2 center(Windows_width/2, Windows_height/2);
    Transition::update();
    float time = m_timeLimit * (1.0-m_p);
    int toLaunch = SizeX*SizeY * (-1.0 + 1.5*m_p)
                    + m_ordre.size();

    for( int i=0; i<toLaunch; ++i) {
        if( m_ordre.empty()) break;
        int iSprite = m_ordre.back();
        m_ordre.pop_back();
        to_plot.push_back(iSprite);
        Vector2 posDeb;
        posDeb.SetFromAngleDegrees(m_p*360);
        posDeb *= static_cast<float>(Windows_width*0.5);
        posDeb += center;
        m_mixer.add(  new TweenerLin<Vector2>(
                &(m_listeSprite[iSprite].pos),
                posDeb,
                m_listeSprite[iSprite].pos,
                time*0.7) );
        m_mixer.add(  new TweenerLin<float>(
                &(m_listeSprite[iSprite].angleZ),
                Random::Float(-360.0, 360),
                0.0,
                time*0.7) );

    }

    m_mixer.update();
}*/