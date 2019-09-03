#ifndef JUKEBOX_HEADER
#define JUKEBOX_HEADER

#include <vector>
#include <string>

class Jukebox {
    private:
        bool m_isrunning;
        std::vector<std::string> m_tags;
    
    public:
        Jukebox() : m_isrunning(false) {}
        virtual ~Jukebox() {}

        void play();
        void stop();
        void check();

        void add_tag(const std::string& txt);
        void clear_tag();
};

#endif