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

void Interval::finish() {
    m_isFinished = true;
    _finish();
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


IntervalAction::IntervalAction(float t) :
    m_execStart([]{}),
    m_execFinish([]{}),
    m_timeElapsed(0.0),
    m_timeLimit(t)
{ }

IntervalAction::IntervalAction(Interval::FuncType f) :
    m_execStart(f),
    m_execFinish([]{}),
    m_timeElapsed(0.0),
    m_timeLimit(-1.0)
{ }

void IntervalAction::_start() {
    m_execStart();
    m_timeElapsed = 0.0;
}

void IntervalAction::_finish() {
    m_timeElapsed = m_timeLimit;
    m_execFinish();
}

bool IntervalAction::_update() {
    m_timeElapsed += m_dt;

    if (m_timeElapsed >= m_timeLimit)
        finish();

    float p = (m_timeElapsed/m_timeLimit);
    _doAction(p);
    return isFinished();
}


void IntervalContainer::add(Interval *i) {
    m_liste.push_back(PtrInterval(i));
}

void IntervalContainer::clear() {
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

void Sequence::_finish() {
    m_itCurrent = m_liste.end();
}

void Parallele::_start() {
    for(auto &ptr:m_liste) ptr->start();
}

void Parallele::_finish() {
    for(auto &ptr:m_liste) ptr->finish();
}

bool Parallele::_update() {
    bool finished = true;
    iterator it = m_liste.begin();
    for(; it != m_liste.end(); ++it)
        finished &= (*it)->update(m_dt);

    if( finished) finish();

    return finished;
}

void Organizer::start() {
    for(auto &iptr:m_list) iptr->start();
    for(auto &item: m_map) item.second->start();
}

void Organizer::add(Interval *interval) {
    if( m_isRunning) interval->start();
    m_list.push_back(PtrInterval(interval));
}

void Organizer::update_list(float dt) {
    m_list.remove_if(
        [dt](PtrInterval ptr) {
            return ptr->update(dt);
        }
    );
}

void Organizer::update_map(float dt) {
    m_map.erase(
        remove_if(
            m_map.begin(),
            m_map.end(),
            [dt](PtrInterval ptr) {
                return ptr->update(dt);
            }
        ),
        m_map.end()
    ); 
}

void Organizer::update(float dt) {
    if( !m_list.empty()) update_list(dt);
    if( !m_map.empty()) update_map(dt);
}

void Organizer::clear() {
    m_list.clear();
    m_map.clear();
}
