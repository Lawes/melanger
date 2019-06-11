#ifndef PSM_RENDER_H_INCLUDED
#define PSM_RENDER_H_INCLUDED

#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>

#include "psm_particule.h"


namespace PSM
{

typedef float GLmatrix16f[16];

class Render
{
public:
    Render(sf::PrimitiveType t, unsigned int n);
    virtual ~Render() {};

    void setTexture(const sf::Texture& texture) {
        m_texture = &texture;
        m_xtextsize = texture.getSize().x;
        m_ytextsize = texture.getSize().y;
    }

    void setBlending(sf::BlendMode::Factor src, sf::BlendMode::Factor dst) {
        m_blend = sf::BlendMode(src, dst);
    }

    const sf::Texture* getTexture() const {
        return m_texture;
    }

    virtual void operator() (std::vector<Particule>::const_iterator p, const unsigned numToDraw) const = 0;
    virtual void draw_() const {};
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;

protected:
    sf::BlendMode m_blend;
    const sf::Texture *m_texture;
    mutable sf::VertexArray m_displayBuffer;
    unsigned int m_maxParticuleNum, m_xtextsize, m_ytextsize;

};

class render_POINTS : public Render
{
    public:
        render_POINTS(unsigned int n);

        void operator() (std::vector<Particule>::const_iterator p, const unsigned numToDraw) const;
        virtual void draw_() const;
        void setSize(float s) {
            m_pointSize = s;
        }

    private:
        float m_pointSize;
};

class render_LINES : public Render {
    public:
        render_LINES(unsigned int n);

        void operator() (std::vector<Particule>::const_iterator p, const unsigned numToDraw) const;
        virtual void draw_() const;

        void setLineWidth(float s) {
            m_lineWidth = s;
        }

    private:
        float m_lineWidth;

};

class render_SPRITES : public Render
{
public:
	render_SPRITES(unsigned int n);
    virtual ~render_SPRITES() { }

    void operator() (std::vector<Particule>::const_iterator p, const unsigned numToDraw) const;
};

}

#endif // PSM_RENDER_H_INCLUDED
