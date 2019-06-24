#ifndef BOARDSHAPE_HEADER
#define BOARDSHAPE_HEADER

#include <SFML/Graphics.hpp>

class BoardShape : public sf::Drawable {
    public:
        struct State {
            int id, ipos, irot;
            bool isMoving, isRotating, isSeleted, isOvermouse;
        };

    private:
        State m_state;
        sf::VertexArray m_pt, m_cadre, m_select;
        sf::Vector2f m_translate;
        float m_theta, m_scale;
        sf::Texture *m_bord, *m_img;

        static float _getRotation(int irot) {
            return static_cast<float>(irot)*90.0f;
        }

    public:

        float getRotationDeg() const {
            return _getRotation(m_state.irot);
        }

        BoardShape();

        State& getState() {
            return m_state;
        }

        bool good() const;
        bool isFixed() const;
        void moveTo(int newipos );
        void rotatePlus();
        void rotateMinus();
        void endMove();
        void endRotation();

        void init(int id, int ipos, int rotation, const sf::Vector2f& v, float size);

        void select(bool b) { m_state.isSeleted = b; }
        bool isSelected() const { return m_state.isSeleted;}
        void overMouse(bool b) { m_state.isOvermouse = b;}
        bool isOverMouse() const { return m_state.isOvermouse;}
        sf::Vector2f *getTranslate() { return &m_translate; }
        float *getRotate() { return &m_theta; }

        void setTexture(sf::Texture *img, sf::Texture *bord);
        void setTextcoords(const sf::FloatRect& rect);
        void setColor( const sf::Color& color);

        void setTransform(const sf::Vector2f& t, float angle, float scale);
        void setTranslate( const sf::Vector2f& t) { m_translate = t; }
        void update();

        void doTransformation(float angle, float scale, const sf::Vector2f& t);

        bool in(const sf::Vector2f& p) const;

        void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

#endif
