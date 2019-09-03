#include "jukebox.h"
#include "globals.h"
#include "libwam/random.h"

using namespace std;

void Jukebox::add_tag(const string& txt) {
    m_tags.push_back(txt);
}

void Jukebox::clear_tag() {
    m_tags.clear();
}

void Jukebox::play() {
    m_isrunning = true;
}

void Jukebox::stop() {
    m_isrunning = false;
    MIXER.nomusic();
}

void Jukebox::check() {
    if( !m_isrunning)
        return;

    if( !MIXER.isPlayingMusic()) {
        auto it = Random::randIterator(m_tags.begin(), m_tags.size());
        MIXER_MUSIC(*it);
    }

}