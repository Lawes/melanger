#include "mixer.h"
#include <iostream>

using namespace std;

Mixer::Mixer() :
    m_maxsounds(10),
    m_sounds(m_maxsounds)
{ }

void Mixer::initTime() {
    Timeit::Time t = Timeit::getTime();
    for(auto &s: m_sounds) {
        s.time = t;
    }
}

void Mixer::setVolume(int volume) {
    sf::Listener::setGlobalVolume(volume);
}

void Mixer::nosound() {
    for(auto &s:m_sounds) {
        s.sound.stop();
    }
    m_music.stop();
}

size_t Mixer::_findSound() const {
    std::size_t imin=0;
    Timeit::Time mint = Timeit::getTime();

    for(size_t i=0; i<m_sounds.size(); ++i) {
        if(m_sounds[i].sound.getStatus() != sf::SoundSource::Playing) {
            imin = i;
            break;
        }
        if(m_sounds[i].time < mint) {
            mint = m_sounds[i].time;
            imin = i;
        }
    }
    return imin;
}

void Mixer::play(sf::SoundBuffer& buffer, float vol, bool loop) {
    size_t imin = _findSound();

    m_sounds[imin].sound.setBuffer(buffer);
    m_sounds[imin].sound.setVolume(vol);
    m_sounds[imin].sound.setLoop(loop);
    m_sounds[imin].sound.play();
    m_sounds[imin].time = Timeit::getTime();
}

void Mixer::music(const std::string& filename, float vol, bool loop) {
    if( !m_music.openFromFile(filename)) {
        cerr << "Error playing music: " << filename << " can't be opened" << endl;
        return;
    }
    m_music.setVolume(vol);
    m_music.setLoop(loop);
    m_music.play();
}