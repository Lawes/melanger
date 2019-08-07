#ifndef RESSOURCEMANAGER_HEADER
#define RESSOURCEMANAGER_HEADER

#include <map>
#include <string>
#include <iostream>
#include <memory>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "manager.h"
#include "anim.h"



#define IMPLEMENT_Ressouce(type, name) \
        public: \
            typedef type name; \
            typedef Manager<name> M ## name; \
            bool add ## name (const std::string& id, const std::string& path) { \
                addName(id, path); \
                auto ptr = M ## name::PtrType(new name()); \
                bool res = ptr->loadFromFile(path); \
                if(res) m_ ## name.add(id, std::move(ptr)); \
                else { \
                    std::cerr << "unable to load : " << path << " !" << std::endl; } \
                return res; \
            } \
        private: \
            M ## name m_ ## name; \
        public: \
            bool get(const std::string& id, name* &ressource) { \
                ressource = m_ ## name.get(id); \
                return ressource != nullptr; \
            } \
            name* get ## name (const std::string& id) {\
                return m_ ## name.get(id); \
            }


class RessourceManager {
    IMPLEMENT_Ressouce(sf::Texture, Texture)

    IMPLEMENT_Ressouce(sf::Font, Font)

    IMPLEMENT_Ressouce(sf::SoundBuffer, Sound)

    IMPLEMENT_Ressouce(sf::Image, Image)

    public:
        typedef Manager<Anim> MAnim;
        bool addAnimations(const std::string& filename);
        bool get(const std::string& id, Anim* &ressource) {
            ressource = m_anim.get(id);
            return ressource != nullptr;
        }

        bool addName(const std::string& id, const std::string& filename) {
            m_names[id] = filename;
            return true;
        }

        std::string getName(const std::string& id) {
            auto it = m_names.find(id);
            if( it == m_names.end())
                return "";
            return it->second;
        }

    private:
        std::map<std::string, std::string> m_names;
        MAnim m_anim;
};

#undef IMPLEMENT_Ressouce

#endif