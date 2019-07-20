#include "transition_ext.h"


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
    m_currenttime=0.0;
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




/*
void FonduTransition::_begin() {

}

void FonduTransition::_end() {

}

void FonduTransition::update(float dt) {

}

void FonduTransition::draw(sf::RenderTarget &win) const {
}

void FonduTransition::load() {}


 */

/*
void Transition::update() {
    m_timeElapsed += VODZO::deltaTime;

    if( m_timeElapsed >= m_timeLimit) {
        m_timeElapsed=m_timeLimit;
        end();
    }
    m_p = m_timeElapsed/m_timeLimit;
}


void Transition::draw() {
    int x = 700;
    int y = 0;
    int w = 50;
    int h = static_cast<int>(500.0 );

    glColor3d(0, 0, 1);
    glBegin(GL_QUADS);
    glVertex2i(x,y);
    glVertex2i(x+w,y);
    glVertex2i(x+w,y+h);
    glVertex2i(x,y+h);
    glEnd();

    glColor3d(0, 1, 0);
    glBegin(GL_LINE_LOOP);
    glVertex2i(x,y);
    glVertex2i(x+w,y);
    glVertex2i(x+w,y+h);
    glVertex2i(x,y+h);
    glEnd();


}

void FonduTransition::_init() {
    load(m_context->getCurrent(), m_context->getNext());

    m_color = Color(1,1,1,0);

}




void FonduTransition::draw() {
//    cout << m_p << endl;
    m_color.a = 1.0 - m_p;
    glColor4fv(&m_color.r);
    m_sp1.draw(0,0,
        Windows_width,
        Windows_height);

    m_color.a = m_p;
    glColor4fv(&m_color.r);

    m_sp2.draw(0,0,
        Windows_width,
        Windows_height);

}

const int VerreTransition::SizeX = 10;
const int VerreTransition::SizeY = 7;


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