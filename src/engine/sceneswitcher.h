#ifndef SCENE_SWITCHER_HEADER
#define SCENE_SWITCHER_HEADER

#include <string>
#include <memory>
#include <list>

#include <SFML/Graphics.hpp>

#include "scene.h"
#include "input.h"
#include "manager.h"

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