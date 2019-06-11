
#include <SFML/Graphics.hpp>

#include "widget.h"
#include "decorator_ext.h"
#include "globals.h"


void simpleBox(gui::Widget &w) {
    w.getRenderSelect().setOutlineColor(sf::Color::Magenta);
    w.getRenderSelect().setOutlineThickness(4.0);
    w.getRenderSelect().setFillColor(sf::Color::Green);
    w.getRenderBG().setFillColor(sf::Color::Yellow);
}

void underline(gui::Widget &w) {
    sf::Texture *t;
    RM.get("underline", t);
    w.getRenderSelect().setOrigin(0,-5);
    w.getRenderSelect().setTexture(t);
    w.getRenderSelect().setFillColor(sf::Color(255,255,0,100));
}

void textbackground(gui::Widget &w) {
    sf::Texture *t;
    w.getRenderBG().setFillColor(sf::Color(250,150,150,200));
    RM.get("txtbg", t);
    w.getRenderBG().setTexture(t);

    auto &box = w.getBox();
    const float alpha = 0.2f;
    w.move(-alpha*box.width, -2.0f*alpha*box.height);
    box.width *= 1.0f+2*alpha;
    box.height *= 1.0f+4*alpha;
    //w.getRenderBG().setOutlineThickness(4.0);
    //w.getRenderBG().setOutlineColor(sf::Color::Magenta);
}

void lightbg(gui::Widget &w) {
    w.getRenderBG().setFillColor(sf::Color::Black);
    w.getRenderBG().setOutlineColor(sf::Color(250,50,50,200));
    w.getRenderBG().setOutlineThickness(7);
}

void turbobg(gui::Widget &w) {
    sf::Texture *t;
    w.getRenderBG().setFillColor(sf::Color(100,100,150,100));
    RM.get("turbocadre", t);
    w.getRenderBG().setTexture(t);

    auto &box = w.getBox();
    const float alpha = 0.2f;
    box.height *= 1.0f+2*alpha;

}

void loadDecorators() {
    DECOM.add("test", simpleBox);
    DECOM.add("underline", underline);
    DECOM.add("txtbg", textbackground);
    DECOM.add("lightbg", lightbg);
    DECOM.add("turbobg", turbobg);
}