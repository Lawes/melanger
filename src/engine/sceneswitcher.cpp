#include "sceneswitcher.h"

#include <string>

using namespace std;

SceneSwitcher::SceneSwitcher(sf::RenderWindow& win) :
    m_win(win),
    m_wantExit(false),
    m_switch(false),
    m_in(win)
{ }

SceneSwitcher::~SceneSwitcher()
{ }

sf::FloatRect SceneSwitcher::getBox() const {
    sf::Vector2f origin(m_win.mapPixelToCoords(sf::Vector2i(0,0)));
    auto size =  m_win.getSize();
    auto s= sf::FloatRect(origin.x, origin.y, size.x, size.y);
    //cout << "context box : " << s.left << " " << s.top << " " << s.height << " " << s.width << endl;
    return s;
}

void SceneSwitcher::drawSceneName(const std::string& name, sf::RenderTarget& win) {
    win.clear();
    Scene * s =  m_scenes.get(name);
    if( s == nullptr)
        return;
    
    s->begin();
    s->draw(win);
    s->end();
}


void SceneSwitcher::draw(sf::RenderTarget& win) {
    win.clear();
    for(auto it = m_onthefly.rbegin(); it != m_onthefly.rend(); ++it)
        (*it)->draw(win);

    for(auto it = m_overlay.rbegin(); it != m_overlay.rend(); ++it)
        (*it)->draw(win);
}


void SceneSwitcher::switchSceneWithTransition(const string& scenename, const string& transname) {
    TransitionScene *ts = m_transscenes.get(transname);
    if(ts == nullptr) {
        switchScene(scenename);
        return;
    }
    ts->setFromToTexture(scenename);
    endAllScene();
    ts->begin();
    m_onthefly.push_back(ts);
    m_next = scenename;
}

void SceneSwitcher::switchScene(const std::string& name) {
    endAllScene();
    m_next = name;
}


void SceneSwitcher::endAllScene() {
    for(auto e:m_overlay) if(! e->isEnded()) e->end();
    //m_overlay.clear();
    for(auto e:m_onthefly) if(! e->isEnded()) e->end();
    //m_onthefly.clear();
}

void SceneSwitcher::pushScene(const string& name) {
    Scene * s =  m_scenes.get(name);
    if( s == nullptr ) {
        cout << "scene : " << name << "do not exists !" << endl;
        return;
    }
    s->begin();
    m_onthefly.push_back(s);   
}

void SceneSwitcher::pushOverlay(const string& name) {
    Scene * s =  m_scenes.get(name);
    if( s == nullptr ) {
        cout << "overlay : " << name << "do not exists !" << endl;
        return;
    }
    s->begin();
    m_overlay.push_back(s); 
}

void SceneSwitcher::run() {
    //cout << ">>> begin update <<<" << endl;
    float deltaTime = m_clock.restart().asSeconds();
    if(m_in.update() ) {
        exit();
        return;
    }

    if( !m_overlay.empty()) {
        for(auto it = m_overlay.rbegin(); it != m_overlay.rend(); ++it)
            (*it)->check_events(m_in);

        for(auto it = m_overlay.begin(); it != m_overlay.end(); ++it) {
            (*it)->update(deltaTime);
            if( (*it)->isEnded()) {
                (*it)->end();
                m_overlay.erase(it--);
            }
        }
    }
    else {
        //cout << "check event" << endl;
        for(auto it = m_onthefly.rbegin(); it != m_onthefly.rend(); ++it)
            (*it)->check_events(m_in);

        //cout << "do update dt" << endl;
        for(auto it = m_onthefly.begin(); it != m_onthefly.end(); ++it) {
            (*it)->update(deltaTime);
            if( (*it)->isEnded()) {
                (*it)->end();
                m_onthefly.erase(it--);
            }
        }
        if( m_onthefly.empty() ) {
            if(m_next.empty())
                return;
            pushScene(m_next);
            m_next.clear();
        }
    }
    //cout << "draw" << endl;
    draw(m_win);
    m_win.display();
}