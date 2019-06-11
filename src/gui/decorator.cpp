
#include "decorator.h"

using namespace gui;
using namespace std;

bool DecoratorManager::apply(const string& id, Widget& w) const {
    auto it = m_content.find(id);
    if( it == m_content.end())
        return false;

    it->second(w);
    return true;
}

void DecoratorManager::add(const string& id, DecoratorManager::FuncWidget func) {
    m_content[id] = func;
}

bool  DecoratorManager::hasId(const string& id) const {
    return m_content.find(id) == m_content.end();
}