#include "shapecontainer.h"
#include "trigocossin.h"

using namespace std;

namespace {
	TrigoCosSin tableCosSin(2);

}

ShapeContainer::ShapeContainer():
    m_va(sf::Quads)
 {}

void ShapeContainer::clear() {
    m_va.clear();
    m_shapes.clear();

}

void ShapeContainer::update(std::size_t i) {
    Shape &shape = m_shapes[i];

    sf::Color col(255,255,255, shape.alpha);

    float cosA = tableCosSin.Cos(shape.angle),
        sinA = tableCosSin.Sin(shape.angle);
    
    sf::Vector2f up(cosA * 0.0f + sinA * 1.0f, -sinA * .0f + cosA * 1.0f), 
        side(up.y, -up.x);

	up *= shape.size.y;
	side *= shape.size.x;

    sf::Vertex v1, v2, v3, v4;
    size_t iva = i*4;

    m_va[iva].position = sf::Vector2f(shape.pos.x - side.x - up.x,  shape.pos.y - side.y - up.y);
    m_va[iva].color = col;
    m_va[iva+1].position = sf::Vector2f(shape.pos.x + side.x - up.x,  shape.pos.y + side.y - up.y);
    m_va[iva+1].color = col;
    m_va[iva+2].position = sf::Vector2f(shape.pos.x + side.x + up.x,  shape.pos.y + side.y + up.y);
    m_va[iva+2].color = col;
    m_va[iva+3].position = sf::Vector2f(shape.pos.x - side.x + up.x,  shape.pos.y - side.y + up.y);
    m_va[iva+3].color = col;
}

void ShapeContainer::update() {
    for(size_t i=0; i<m_shapes.size(); ++i)
        update(i);
}


void ShapeContainer::add(sf::IntRect coord, sf::Vector2f pos, sf::Vector2f s, float angle) {
    add(coord, pos, s, angle, 255.0f);
}

void ShapeContainer::add(sf::IntRect coord, sf::Vector2f pos, sf::Vector2f s, float angle, float alpha) {
    m_shapes.emplace_back(pos, s, angle, alpha);
    sf::Vertex v;
    v.texCoords = sf::Vector2f(coord.left, coord.top);
    m_va.append(v);
    v.texCoords = sf::Vector2f(coord.left+coord.width, coord.top);
    m_va.append(v);
    v.texCoords = sf::Vector2f(coord.left+coord.width, coord.top+coord.height);
    m_va.append(v);
    v.texCoords = sf::Vector2f(coord.left, coord.top+coord.height);
    m_va.append(v);
}

void ShapeContainer::draw(sf::RenderTarget &win) const {
    win.draw(m_va, m_texture);
}