#ifndef EVENTS_DISPACHER_HEADER
#define EVENTS_DISPACHER_HEADER

#include <list>
#include <functional>
#include <memory>

#include <SFML/Graphics.hpp>

class CInput;

class gEvent {
    public:
        typedef std::function<void(void)> EventFunc;
        typedef sf::FloatRect Box;

    protected:
        EventFunc m_action_true, m_action_false;

    public:
        gEvent(EventFunc ftrue) : 
            m_action_true(ftrue), m_action_false([]{}) {};
        gEvent(EventFunc ftrue, EventFunc ffalse) : 
            m_action_true(ftrue), m_action_false(ffalse) {};

        virtual ~gEvent() {}
        void check(CInput& in) { 
            if( check_(in) ) m_action_true(); 
            else m_action_false();
        };
    private:
        virtual bool check_(CInput& in) { return true;}
};

class MouseMotionEvent : public gEvent {
    private:
        Box &m_box;
        virtual bool check_(CInput& in);

    public:
        MouseMotionEvent(Box &w, EventFunc ftrue, EventFunc ffalse) 
            : gEvent(ftrue, ffalse), m_box(w)  {};
};

class KeyBoxEvent : public gEvent {
    private:
        sf::Keyboard::Key m_key;
        Box &m_box;
        virtual bool check_(CInput& in);

    public:
        KeyBoxEvent(sf::Keyboard::Key k, Box &w, EventFunc func)
            : gEvent(func), m_key(k), m_box(w) {};
};

class SimpleKeyEvent : public gEvent {
    private:
        sf::Keyboard::Key m_key;
        virtual bool check_(CInput& in);

    public:
        SimpleKeyEvent( sf::Keyboard::Key key, EventFunc func) 
            : gEvent(func), m_key(key) {}
};

class MouseBoutonEvent : public gEvent {
    private:
        sf::Mouse::Button m_button;
        Box &m_box;
        virtual bool check_(CInput& in);

    public:
        MouseBoutonEvent(sf::Mouse::Button b, Box &w, EventFunc f) 
            : gEvent(f), m_button(b), m_box(w) {};
};

class EventDispatcher {
    private:
        typedef std::unique_ptr<gEvent> PtrEvent;
        typedef std::list< PtrEvent > ListEvents;
        ListEvents m_listeEvents;
        bool m_check_events;

    public:
        EventDispatcher() : m_check_events(true) {}
        virtual ~EventDispatcher() { clearEvents(); }

        void enable_check_events() { m_check_events=true;}
        void disable_check_events() { m_check_events=false;}

        void add_event(sf::Keyboard::Key key, gEvent::EventFunc ftrue);
        void add_event(sf::Keyboard::Key key, sf::FloatRect& box, gEvent::EventFunc ftrue);
        void add_event(sf::Mouse::Button button, sf::FloatRect& box, gEvent::EventFunc ftrue);
        void add_event(sf::FloatRect& box, gEvent::EventFunc ftrue, gEvent::EventFunc ffalse);

        void check_events(CInput& in);

        void clearEvents();
};

#endif
