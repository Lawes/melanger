#ifndef SHAPE_HEADER
#define SHAPE_HEADER

#include "Vector2.h"
#include "Color.h"


struct Rect {
    VODZO::Vector2 p1, p2, p3, p4;

    void doTransformation(const float angle, 
        const float scale, const VODZO::Vector2& trans);

    bool in( const VODZO::Vector2& p);

};


class Shape {
private:
    int id_;
    Rect m_pt, m_cadre;

    VODZO::Color color_;
    unsigned int idTexture_, ;

    VODZO::Vector2 translate_;
    float theta_, scale_;
	bool selected_;

    static int currentId;
    static unsigned int idCadre_;

public:
    enum TransType{
        TRANSLATION = 0,
        ROTATION=1
    };

    static void ClearId() {
        currentId = 0;
    }

    static void setIdCadre(unsigned int id) {
        idCadre_ = id;
    }

    Shape();
    Shape(const unsigned int id);

    int id(TransType t) {
        return id_*2 + t;
    }

    int id() {
        return id_;
    }

    VODZO::Vector2 *getTranslate() {
        return &translate_;
    }

    float *getRotate() {
        return &theta_;
    }

    void setIdTexture(unsigned int id) {
        idTexture_ = id;
    }

    unsigned int getIdTexture() {
        return idTexture_;
    }

    void setColor( const VODZO::Color& color) {
        color_ = color;
    }

	void set_select(bool b) { selected_ = b; }

    void setTransform(const VODZO::Vector2& t, const float angle, const float scale);
    void setTranslate( const VODZO::Vector2& t) { translate_ = t; }
    void apply();
    bool in(const VODZO::Vector2& p);
    void draw();

};

#endif
