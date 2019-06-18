#ifndef MIXER_HEADER
#define MIXER_HEADER

#include <vector>
#include <string>
#include <SFML/Audio.hpp>

#include "timeit.h"

class Mixer {
    private:
        struct SoundTime {
            sf::Sound sound;
            Timeit::Time time;
        };
        std::size_t m_maxsounds;
        std::vector<SoundTime> m_sounds;
        sf::Music m_music;

        size_t _findSound() const;
    
    public:
        Mixer();

        void initTime();
        void nosound();
        void play(sf::SoundBuffer& buffer, float vol=100.0f, bool loop=false);

        void setVolume(int volume);
};


#endif