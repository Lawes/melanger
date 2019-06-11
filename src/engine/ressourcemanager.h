#ifndef RESSOURCEMANAGER_HEADER
#define RESSOURCEMANAGER_HEADER

#include <string>
#include <iostream>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "manager.h"
#include "anim.h"

class RessourceManager {
    public:
        typedef sf::Texture texture;
        typedef sf::Font font;
        typedef sf::SoundBuffer sound;
        typedef Anim anim;

        typedef Manager<texture> MTexture;
        typedef Manager<font> MFont;
        typedef Manager<sound> MSound;
        typedef Manager<Anim> MAnim;

        bool addTexture(const std::string& id, const std::string& path);
        bool addAnimations(const std::string& filename);
        bool addFont(const std::string& id, const std::string& path);
        bool addSound(const std::string& id, const std::string& path);

        texture* getTexture(const std::string& id) {
            return m_texture.get(id);
        }

        bool get(const std::string& id, texture* &ressource ) {
            ressource = m_texture.get(id);
            return ressource != nullptr;
        }
        bool get(const std::string& id, font* &ressource ) {
            ressource = m_font.get(id);
            return ressource != nullptr;
        }
        bool get(const std::string& id, sound* &ressource ) {
            ressource = m_sound.get(id);
            return ressource != nullptr;
        }
        bool get(const std::string& id, anim* &ressource) {
            ressource = m_anim.get(id);
            return ressource != nullptr;
        }

    private:
        MTexture m_texture;
        MFont m_font;
        MSound m_sound;
        MAnim m_anim;
};

#endif