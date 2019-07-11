#ifndef SCENE_HEADER
#define SCENE_HEADER

#include "eventdispatcher.h"
#include "input.h"
#include <SFML/Graphics.hpp>
#include <string>

class SceneSwitcher;

#define INTERFACE_Scene(terminal) \
    public: \
        virtual void load() terminal \
        virtual void update(float dt) terminal \
        virtual void draw(sf::RenderTarget &win) const terminal \
    private: \
        virtual void _begin() terminal \
        virtual void _end() terminal

//#define BASE_GameObject INTERFACE_GameObject(=0;)
#define BASE_Scene INTERFACE_Scene({};)
#define DERIVED_Scene INTERFACE_Scene(;)


class Scene : public EventDispatcher {
    public:
        typedef std::function<void(void)> Func;
    private:
        bool m_isEnded;
        Func m_beginfunc, m_endfunc;
        std::string m_name;
    protected:
        SceneSwitcher *m_context;
        CInput &in;

    public:
        Scene(SceneSwitcher* parent);
        Scene(SceneSwitcher* parent, const std::string& txt);

        virtual ~Scene() {}
        void begin() { m_isEnded = false; m_beginfunc(); _begin(); }
        void toggle() { m_isEnded = !m_isEnded;}
        void end() { m_isEnded = true; m_endfunc(); _end(); }
        bool isEnded() { return m_isEnded; }

        void setEndFunc(Func f) { m_endfunc = f; }
        void setBeginFunc(Func f) { m_beginfunc = f; }
        void setContext(SceneSwitcher * context) { m_context = context; }

        void setName(const std::string& txt) {
            m_name = txt;
        }
        std::string getName() const{
            return m_name;
        }

    BASE_Scene
};
#endif
