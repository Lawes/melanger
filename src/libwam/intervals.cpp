#include "intervals.h"
#include <iostream>
#include <algorithm>

using namespace std;

Interval::~Interval() {
    //std::cout << "clear Interval" << std::endl;
}

void Interval::start() {
    m_inPause = false;
    m_isFinished = false;
    _start();
}

bool Interval::update(float dt) {
    m_dt = dt;
    if( m_isFinished ) return true;
    if( m_inPause ) return false;

    bool finishQ = _update();
    if( finishQ && m_loop) {
        start();
        finishQ = false;
    }
    return finishQ;
}

void IntervalAction::_start() {
    m_timeElapsed = 0.0;
    m_p = 0.0;
}

void IntervalAction::finish() {
    Interval::finish();
    m_timeElapsed = m_timeLimit;
}

bool IntervalAction::_update() {
    m_timeElapsed += m_dt;

    if (m_timeElapsed >= m_timeLimit)
        finish();

    float m_p = (m_timeElapsed/m_timeLimit);
    _doAction(m_p);
    return isFinished();
}


void IntervalContainer::add(Interval *i) {
    m_liste.push_back(PtrInterval(i));
}

void IntervalContainer::clear() {
    //std::cout << "clear IntervalContainer" << std::endl;
    //for( auto it:m_liste) delete it;
    //for_each(m_liste.begin(), m_liste.end(), Private::DeleteObject());
    m_liste.clear();
}

void Sequence::_start() {
    m_itCurrent = m_liste.begin();
    if( m_itCurrent == m_liste.end()) {
        finish();
        return;
    }

    (*m_itCurrent)->start();
}

bool Sequence::_update() {
    if( m_itCurrent == m_liste.end() ) return false;
    bool state = (*m_itCurrent)->update(m_dt);

    if( state ) {
        ++m_itCurrent;
        if( m_itCurrent == m_liste.end()) {
            finish();
            return true;
        }

        (*m_itCurrent)->start();
    }
    return false;
}

bool Sequence::isFinished() {
    return (m_itCurrent==m_liste.end());
}

void Sequence::finish() {
    Interval::finish();
    m_itCurrent = m_liste.end();
}

void Parallele::_start() {
    for(auto &ptr:m_liste) ptr->start();
}

void Parallele::finish() {
    Interval::finish();
    for(auto &ptr:m_liste) ptr->finish();
}

bool Parallele::_update() {
    bool finished = true;
    iterator it = m_liste.begin();
    for(; it != m_liste.end(); ++it)
        finished &= (*it)->update(m_dt);

    if( finished)
        Interval::finish();

    return finished;

}

void Organizer::start() {
    for(auto &iptr:m_liste)
        iptr->start();
}

void Organizer::add(Interval *interval) {
    if( m_isRunning) interval->start();
    m_liste.push_back(PtrInterval(interval));
}


void Organizer::update(float dt) {
    std::list<ListInterval::iterator> listeToRemove;

    for (ListInterval::iterator i = m_liste.begin();
         i != m_liste.end(); ++i) {
        if( (*i)->update(dt) ) {
            listeToRemove.push_back(i);
        }
    }

    for (auto i:listeToRemove) {
        //delete *i;
        m_liste.erase(i);
    }
    listeToRemove.clear();
}

void Organizer::clear() {
    //std::cout << "taille = " << m_liste.size() << std::endl;
    /*
    for(auto &it:m_liste) {
        std::cout << "free intervals" << std::endl;
        //delete it;
    }*/
    m_liste.clear();
}
