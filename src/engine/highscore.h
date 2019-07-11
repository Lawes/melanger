#ifndef HIGHSCOREMANAGER_H
#define HIGHSCOREMANAGER_H

#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <ostream>

#include "configreader.h"

template< class T>
class HighScore {
    public:
        typedef T Score;
        typedef std::vector<Score> LvlScore;

        HighScore() {};
        virtual ~HighScore() {}

        void load(const char* filename) {
            ConfigReader device;

            device.read(filename);
            ConfigReader::Entries liste;
            device.getTitles(liste);

            for( auto name:liste) {
                LvlScore scores;
                device.get(name, "score", scores);
                addScore(name, scores);
            }
        }
        void save(std::ostream& flux) const {
            ConfigReader device;

            for(auto &lvl: m_allScores) {
                for(auto& val:lvl.second)
                    device.add(lvl.first, "score", val);
            }
            device.write(flux);
        }

        void addScore(const std::string& lvlname, Score s){
            auto it = m_allScores.find(lvlname);

            if( it == m_allScores.end())
                it = m_allScores.insert({lvlname, LvlScore()}).first;
            
            it->second.push_back(s);
        }

        template<class Container>
        void addScore(const std::string& lvlname, const Container& values) {
            auto it = m_allScores.find(lvlname);
            if( it == m_allScores.end()) {
                it = m_allScores.insert({lvlname, LvlScore()}).first;
            }

            for(auto v:values)
                it->second.push_back(v);
        }

        LvlScore getScore(const std::string& lvlname, std::size_t maxval, bool reverse=false) const {
            auto it = m_allScores.find(lvlname);
            if( it == m_allScores.end())
                return HighScore::LvlScore();
            
            HighScore::LvlScore res(it->second);
            if( reverse)
                std::sort(res.begin(), res.end(), std::less<Score>());
            else
                std::sort(res.begin(), res.end(), std::greater<Score>());
            
            if( maxval > 0)
                res.resize(std::min(maxval, res.size()));
            return res;
        }

    private:
        std::map<std::string, LvlScore> m_allScores;
};

#endif // HIGHSCOREMANAGER_H
