#include "sceneswitcher.h"
#include "transitionscene.h"
#include <SFML/Graphics.hpp>

using namespace std;

void TransitionScene::setFromToTexture(const string &nextname) {

    sf::RenderTexture rt;
    rt.setView(m_context->getView());
    sf::FloatRect rect =  m_context->getBox();
    rt.create(rect.width, rect.height);

    m_context->draw(rt);

    rt.display();

    m_from = rt.getTexture();

    rt.clear();
    m_context->drawSceneName(nextname, rt);
    rt.display();

    m_to = rt.getTexture();

}

bool TransitionScene::updateTransitionTime(float dt) {
    m_currenttime += dt;
    m_p = m_currenttime/m_totaltime;
    return m_p > 1.0;
}