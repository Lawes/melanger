#ifndef SHAPE_CONTAINER_HEADER
#define SHAPE_CONTAINER_HEADER

#include <SFML/Graphics.hpp>
#include <vector>

class ShapeContainer {
    private:
        struct Shape {
            sf::Vector2f pos, size;
            float angle, alpha;

            Shape(sf::Vector2f v, sf::Vector2f s, float b, float c) :
                pos(v), size(s), angle(b), alpha(c) {}
        };

        sf::VertexArray m_va;
        sf::Texture *m_texture;
        std::vector<Shape> m_shapes;
    public:
        ShapeContainer();
        virtual ~ShapeContainer() {}

        void clear();

        std::size_t size() const {
            return m_shapes.size();
        }

        Shape& get(std::size_t i) {
            return m_shapes[i];
        }

        void update(std::size_t i);
        void update();

        void setTexture(sf::Texture *t) {
            m_texture = t;
        }
        
        void draw(sf::RenderTarget &win) const;
        void add(sf::IntRect coord, sf::Vector2f pos, sf::Vector2f s, float angle);
        void add(sf::IntRect coord, sf::Vector2f pos, sf::Vector2f s, float angle, float alpha);

};

#endif