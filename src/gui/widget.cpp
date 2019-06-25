
#include <iostream>
#include <algorithm>

#include "widget.h"

using namespace gui;
using namespace std;

void displayBox(const Widget::Box& box) {
    cout << "box : " << box.left << ", " << box.top;
    cout << ", " << box.width << ", " << box.height << endl;
}

void encastre(Widget::Box &box, const Widget::Box& ref, float overlap) {
    box.left  = std::min(box.left, ref.left - overlap);
    box.top   = std::min(box.top, ref.top - overlap);
    box.width = std::max( box.width, ref.left -box.left + ref.width + overlap);
    box.height= std::max( box.height, ref.top -box.top + ref.height + overlap);
}

Widget::Widget(): 
    m_isSelected(false),
    m_isVisible(true),
    m_drawText(false),
    m_fixedsize(false)
{
    m_render_bg.setFillColor(sf::Color::Transparent);
    m_render_select.setFillColor(sf::Color::Transparent);
}

Widget::~Widget() {

}

void Widget::print() const {
    std::cout << "w area : " << m_area.left 
        << ", " << m_area.top << ", " << m_area.width
        << ", " << m_area.height << std::endl;
    std::cout << "posbg : " << m_render_bg.getPosition().x << " " <<  m_render_bg.getPosition().y << std::endl;
}

void Widget::move(float x, float y) {
    m_area.left +=x;
    m_area.top  +=y;
}

void Widget::set_pos(float x, float y) {
    m_area.left=x;
    m_area.top = y;
}

void Widget::set_size(float x, float y) {
    m_area.width = x;
    m_area.height = y;
    m_fixedsize=true;
}

void Widget::update() {
    //m_render_bg.setOutlineColor(sf::Color::Red);
    //m_render_bg.setOutlineThickness(1.0);
    m_render_bg.setPosition(m_area.left, m_area.top);
    m_render_bg.setSize(sf::Vector2f(m_area.width, m_area.height));

    if( m_drawText) {
        Widget::Box box(m_text.getLocalBounds());
        auto csize = m_text.getCharacterSize();
        int dy = box.top;
        float x = m_area.left - (box.width + box.left - m_area.width)/2,
            y = m_area.top - (csize - m_area.height)/2;
        m_text.setPosition(x, y);
    }
    m_render_select.setPosition(m_area.left, m_area.top);
    m_render_select.setSize(sf::Vector2f(m_area.width, m_area.height));
}

void Widget::setText(const sf::String& txt, sf::Font &font, int size, bool force) {
    setText(txt, font, size, sf::Color::White, force);
}


void Widget::setText(const sf::String& txt, sf::Font& font, int size, sf::Color col, bool force) {
    m_text.setFont(font);
    m_text.setString(txt);
    m_text.setCharacterSize(size);
    m_text.setFillColor(col);
    m_drawText = !txt.isEmpty();
    if( force || !m_fixedsize) {
        Widget::Box b(m_text.getLocalBounds());
        //cout << txt.toAnsiString() << endl;
        //displayBox(b);
        Widget::set_size(b.width, size+1);
    }
}

void Widget::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    if( !m_isVisible) return;
    target.draw(m_render_bg, states);
    _draw(target, states);
    if( m_drawText) target.draw(m_text, states);
    if(m_isSelected) {
        target.draw(m_render_select, states);}
}

void Widget::enveloppe(const Widget::Box& box, float border) {
    Widget::set_pos(box.left - border, box.top - border);
    Widget::set_size(box.width + 2*border, box.height + 2*border);
}

void Widget::extend(float e) {
    Widget::Box box(m_area);
    enveloppe(box, e);  
}

void Widget::aligneVertival(const Widget::Box& box, Alignement type, float e=0.0f){
    Widget::Box b(getBox());
    switch(type) {
        case VALIGN_BOTTOM:
            b.top = box.top + box.height - b.height - e;
            break;
        case VALIGN_CENTER:
            b.top = box.top + (box.height - b.height)/2;
            break;
        case VALIGN_TOP:
            b.top = box.top + e;
            break;
        default:
            break;
    }
    set_pos(b.left, b.top);
}

void Widget::aligneHorizontal(const Widget::Box& box, Alignement type, float e=0.0f) {
    Widget::Box b(getBox());
    switch(type) {
        case HALIGN_LEFT:
            b.left = box.left + e;
            break;
        case HALIGN_CENTER:
            b.left = box.left + (box.width - b.width)/2;
            break;
        case HALIGN_RIGHT:
            b.left = box.left + box.width - b.width - e;
            break;
        default:
            break;
    }
    set_pos(b.left, b.top);
}

void Widget::juxtaposeVertival(const Widget::Box& box, Alignement type, float overlap=0.0f) {
    Widget::Box b(getBox());
    switch(type) {
        case VALIGN_BOTTOM:
            b.top = box.top + box.height + overlap;
            break;
        case VALIGN_CENTER:
            b.top = box.top + (box.height - b.height)/2;
            break;
        case VALIGN_TOP:
            b.top = box.top - b.height - overlap;
            break;
        default:
            break;
    }
    set_pos(b.left, b.top);
}

void Widget::juxtaposeHorizontal(const Widget::Box& box, Alignement type, float overlap=0.0f) {
    Widget::Box b(getBox());
    switch(type) {
        case HALIGN_LEFT:
            b.left = box.left - b.width - overlap;
            break;
        case HALIGN_CENTER:
            b.left = box.left + (box.width - b.width)/2;
            break;
        case HALIGN_RIGHT:
            b.left = box.left + box.width + overlap;
            break;
        default:
            break;
    }
    set_pos(b.left, b.top);
}

void Widget::center(const Widget::Box& box) {
    aligneVertival(box, gui::VALIGN_CENTER, 0);
    aligneHorizontal(box, gui::HALIGN_CENTER, 0);
}

sf::Vector2f Widget::getCenter() const {
    return sf::Vector2f(m_area.left+m_area.width/2, m_area.top+m_area.height/2);
}
void Panel::_draw(sf::RenderTarget& target, sf::RenderStates states) const{
    for(const auto &it:m_widgets)
        it->draw(target, states);
}

void Panel::set_pos(float pos_x, float pos_y) {
    sf::Vector2f pos(getPos());
    int oldx = pos_x - pos.x;
    int oldy = pos_y - pos.y;
    Panel::move(oldx, oldy);
}

void Panel::move(float dx, float dy) {
    Widget::move(dx, dy);
    for(auto &it:m_widgets)
        it->move(dx, dy);
}

void Panel::update() {
    Widget::update();
    for(auto& ipt: m_widgets)
        ipt->update();
}

void Panel::release() {
    //std::cout << "destroy widgets" << std::endl;
    m_widgets.clear();
}

void Panel::add_child(Widget *w) {
    if( w && w != this) {
        m_widgets.push_back(Widget::PtrWidget(w));
    }
}

bool Panel::find_widget(float x, float y, Widget& w) {
    for(auto &ipt: m_widgets)
        if(ipt->contains(x,y))
            return ipt->find_widget(x, y, w);
    return false;
}

void Panel::print() const {
    Widget::print();
    for(auto &ipt: m_widgets)
        ipt->print();
}

void BarPanel::release()  {
    Panel::release();
    m_use_area = {0.0f, 0.0f, 0.0f, 0.0f};
}

void BarPanel::set_pos(float pos_x, float pos_y) {
    sf::Vector2f pos(getPos());
    int oldx = pos_x - pos.x;
    int oldy = pos_y - pos.y;
    move(oldx, oldy);
}

void BarPanel::move(float dx, float dy) {
    Panel::move(dx, dy);
    m_use_area.left +=dx;
    m_use_area.top +=dy;
}

void BarPanel::set_size(float x, float y) {
    Panel::set_size(x, y);
    m_use_area.width = x;
    m_use_area.height = y;
}

void BarPanel::print() const {
    std::cout << "w use area : " << m_use_area.left 
        << ", " << m_use_area.top << ", " << m_use_area.width
        << ", " << m_use_area.height << std::endl;    
    Panel::print();
}

void HPanel::add_child(Widget *w, Alignement al) {
    sf::Vector2f oldpos(getPos());
    if( m_widgets.empty()) m_use_area.height = w->getBox().height;
    w->juxtaposeHorizontal(m_use_area, HALIGN_RIGHT, m_widgets.empty()?0:m_espace);
    w->aligneVertival(m_use_area, al);

    encastre(m_use_area, w->getBox(), 0);
    Panel::add_child(w);
    enveloppe(m_use_area, m_border);
    BarPanel::set_pos(oldpos.x, oldpos.y);
}

void VPanel::add_child(Widget *w, Alignement al) {
    sf::Vector2f oldpos(getPos());
    if( m_widgets.empty()) m_use_area.width = w->getBox().width;
    w->juxtaposeVertival(m_use_area, VALIGN_BOTTOM, m_widgets.empty()?0:m_espace);
    w->aligneHorizontal(m_use_area, al);

    encastre(m_use_area, w->getBox(), 0);
    Panel::add_child(w);
    enveloppe(m_use_area, m_border);
    BarPanel::set_pos(oldpos.x, oldpos.y);
}
