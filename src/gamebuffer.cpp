#include <fstream>
#include <iostream>

#include "gamebuffer.h"

using namespace std;

GameBuffer::GameBuffer() :
    m_name("normal"),
    m_currentScore(0)
{
    initConfig("assets/config.txt");
    initHighScore("assets/scores.txt");
}

GameBuffer::Parameters GameBuffer::getConfig(const std::string& name) const {
    Parameters p;
    m_config.getParameters(name, p);
    return p;
}

GameBuffer::Parameters GameBuffer::getConfig() const {
    return getConfig(m_name);
}

void GameBuffer::saveScore(GameBuffer::HS::Score score) {
    setCurrentScore(score);
    m_highscore.addScore(m_name, m_currentScore);
    ofstream flux(m_highscorefile.c_str());
    if( flux.is_open() && !flux.bad())
        m_highscore.save(flux);
}

GameBuffer::Parameters GameBuffer::beginRun(const std::string& name) {
    m_name = name;
    m_currentScore = 0;
    return getConfig();
}