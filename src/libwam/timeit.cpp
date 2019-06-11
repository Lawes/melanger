#include "timeit.h"
#include <iostream>

using namespace std;

void Timeit::reset() {
    m_globalcount = 0;
    m_counter.clear();
}

Timeit::Delta Timeit::addTock(const string& key) {
    auto it = m_counter.find(key);
    Delta dt = tock();
    if( it == m_counter.end() ) {
        m_counter[key] = {dt,1};
    }
    else {
        it->second.dt += dt;
        it->second.count++;
    }
    return dt;
}

void Timeit::display(ostream &fout) const {
    fout << "* Display timers (" << m_globalcount<< ')' << endl;
    //fout << "(in ms)" << endl;
    for(auto it: m_counter) {
        double mean = getMean(it.first);
        fout << '(' << it.first << ')' << endl;
        fout << "\ttotal "  << to_ms(it.second.dt) << " ms, ";
        fout << "#" << it.second.count << endl;
        fout << "\tmean  " << mean << " ms" << endl;
    }

}

std::string Timeit::to_string(double t) {
    char buffer[6];
    int secondes, minutes;

    secondes = static_cast<int>(t)%60;
    minutes = static_cast<int>(t - secondes)/60;

    if( minutes > 0)
        sprintf(buffer, "%2d:%02d", minutes, secondes );
    else
        sprintf(buffer, "%5d", secondes );

    return std::string(buffer);
}