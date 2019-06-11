#ifndef SCENE_HEADER
#define SCENE_HEADER

#include "manager.h"
#include "eventdispatcher.h"
#include "input.h"

#include <SFML/Graphics.hpp>

#include <vector>
#include <list>
#include <utility>
#include <functional>
#include <string>

class CInput;

class SceneSwitcher;


#define INTERFACE_Scene(terminal) \
    public: \
        virtual void load() terminal \
        virtual void update(float dt) terminal \
        virtual void draw(sf::RenderWindow &win) const terminal \
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



class SceneSwitcher {
    public:
        typedef std::unique_ptr<Scene> ScenePrt;
    public:
        SceneSwitcher(sf::RenderWindow &win);
        ~SceneSwitcher();

        template<class T>
        T* newScene(const std::string& name) {
            auto newscene = std::unique_ptr<T>(new T(this));
            newscene->setName(name);
            newscene->load();
            T *res = newscene.get();
            m_scenes.add(name, std::move(newscene));
            return res;
        }

        CInput& getInput() {
            return m_in;
        }

        void clearScene(const std::string & name) {
            m_scenes.del(name);
        }

        void endAllScene();

        void switchScene(const std::string& name);
        void pushOverlay(const std::string& name);
        void pushScene(const std::string& name);

        void run();

        void exit() {
            m_wantExit = true;
        }

        bool wantExit() const {
            return m_wantExit;
        }

        sf::FloatRect getBox() const;

        private:
            void draw();

    private:
        sf::RenderWindow& m_win;
        bool m_wantExit, m_switch;
        CInput m_in;
        sf::Clock m_clock;
        std::list<Scene*> m_onthefly, m_overlay;
        Manager<Scene, std::string> m_scenes;
        std::string m_next;
};

#endif
