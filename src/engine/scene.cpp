#include <iostream>
#include <algorithm>
#include "engine/scene.h"
#include "engine/sceneswitcher.h"

using namespace std;

Scene::Scene(SceneSwitcher* parent) : 
    m_isEnded(false), 
    m_beginfunc([]{}),
    m_endfunc([]{}),
    m_name(""),
    m_context(parent),
    in(parent->getInput())
{ }

Scene::Scene(SceneSwitcher* parent, const string& txt) : 
    m_isEnded(false), 
    m_beginfunc([]{}),
    m_endfunc([]{}),
    m_name(txt),
    m_context(parent),
    in(parent->getInput())
{ }