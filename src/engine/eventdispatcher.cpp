#include "eventdispatcher.h"
#include "input.h"
#include <iostream>

using namespace std;

bool MouseMotionEvent::check_(CInput& in) {
    return m_box.contains( in.MouseX(), in.MouseY());
}

bool KeyBoxEvent::check_(CInput& in) {
    bool res = m_box.contains( in.MouseX(), in.MouseY()) && in.Key(m_key);
    if( res) in.unKey(m_key);
    return res;
}

bool SimpleKeyEvent::check_(CInput& in) {
    bool res = in.Key(m_key);
    if(res) in.unKey(m_key);
    return res;
}

bool MouseBoutonEvent::check_(CInput& in) {
    bool res = m_box.contains(in.MouseX(), in.MouseY()) && in.MouseButton(m_button);
    if( res ) in.unMouseButton(m_button);
    return res;
}

void EventDispatcher::add_event(sf::Keyboard::Key key, gEvent::EventFunc ftrue) {
    m_listeEvents.push_back(std::move(PtrEvent(new SimpleKeyEvent(key, ftrue))));
}

void EventDispatcher::add_event(sf::Keyboard::Key key, gEvent::Box& box, gEvent::EventFunc ftrue) {
    m_listeEvents.push_back(std::move(PtrEvent(new KeyBoxEvent(key, box, ftrue))));
}

void EventDispatcher::add_event(sf::Mouse::Button button, gEvent::Box& box, gEvent::EventFunc ftrue) {
    m_listeEvents.push_back(std::move(PtrEvent(new MouseBoutonEvent(button, box, ftrue))));
}
void EventDispatcher::add_event(sf::FloatRect& box, gEvent::EventFunc ftrue, gEvent::EventFunc ffalse) {
    m_listeEvents.push_back(std::move(PtrEvent(new MouseMotionEvent(box, ftrue, ffalse))));
}

void EventDispatcher::check_events(CInput& in) {
    for(auto &e: m_listeEvents)
        e->check(in);
}

void EventDispatcher::clearEvents() {
    cout << "clear eventsDispatcher" << endl;
    m_listeEvents.clear();
}
