#ifndef GAME_BUFFER_HEADER
#define GAME_BUFFER_HEADER

#include <string>
#include <vector>

#include "gameconfig.h"
#include "engine/highscore.h"

class GameBuffer {
    public:
        typedef HighScore<float> HS;
    private:
        std::string m_name, m_gameconfigfile, m_highscorefile;
        GameConfig m_config;
        HighScore<float> m_highscore;

        HS::Score m_currentScore;

    public:
        GameBuffer();

        void beginRun(const std::string& name);

        void initConfig(const std::string& file) {
            m_gameconfigfile = file;
            m_config.load(file.c_str());
        }
        void initHighScore(const std::string& file) {
            m_highscorefile = file;
            m_highscore.load(file.c_str());
        }

        HS& getHighScore() {
            return m_highscore;
        }

        void saveScore(HS::Score score);

        void setCurrentScore(HS::Score score) {
            m_currentScore = score;
        }

        HS::Score getCurrentScore() const {
            return m_currentScore;
        }

        GameConfig::Parameters getConfig() const;
};

#endif