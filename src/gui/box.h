#ifndef BOX_HEADER
#define BOX_HEADER

#include <algorithm>
#include <SFML/Graphics.hpp>

namespace gui {

template<class T>
class TBox : public sf::Rect<T> {
    typedef T type;
    enum Alignement {
        HALIGN_LEFT,
        HALIGN_CENTER,
        HALIGN_RIGHT,
        VALIGN_TOP,
        VALIGN_CENTER,
        VALIGN_BOTTOM
    };

    TBox(): Rect() {};

    void setPos(const TBox<T>& box) { left=box.left; top=box.top; }

    void encastre(const TBox<T>& box, T overlap = T(0)){
        left = std::min(left, box.left - overlap);
        top = std::min(top, box.top - overlap);
        width = std::max( width, box.left -left + box.width + overlap);
        height = std::max( height, box.top -top + box.height + overlap);
    }

    void enveloppe(const TBox<T>& box, T border) {
        left = box.left - border;
        top = box.top - border;
        width = box.width + 2*border;
        height = box.height + 2*border;
    }

    void aligneVertival(const TBox<T>& box, Alignement type, T e=T(0)){
        switch(type) {
            case VALIGN_BOTTOM:
                top = box.top + box.height - height - e;
                break;
            case VALIGN_CENTER:
                top = box.top + (box.height - height)/2;
                break;
            case VALIGN_TOP:
                top = box.top + e;
                break;
            default:
                break;
        }
    }

    void aligneHorizontal(const TBox<T>& box, Alignement type, T e=T(0)) {
        switch(type) {
            case HALIGN_LEFT:
                left = box.left + e;
                break;
            case HALIGN_CENTER:
                left = box.left + (box.width - width)/2;
                break;
            case HALIGN_RIGHT:
                left = box.left + box.width - width - e;
                break;
            default:
                break;
        }
    }
    void juxtaposeVertival(const TBox<T>& box, Alignement type, T overlap=T(0)) {
        switch(type) {
            case VALIGN_BOTTOM:
                top = box.top + box.height + overlap;
                break;
            case VALIGN_CENTER:
                top = box.top + (box.height - height)/2;
                break;
            case VALIGN_TOP:
                top = box.top - height - overlap;
                break;
            default:
                break;
        }
    }
    void juxtaposeHorizontal(const TBox<T>& box, Alignement type, T overlap=T(0)) {
        switch(type) {
            case HALIGN_LEFT:
                left = box.left - width - overlap;
                break;
            case HALIGN_CENTER:
                left = box.left + (box.width - width)/2;
                break;
            case HALIGN_RIGHT:
                left = box.left + box.width + overlap;
                break;
            default:
                break;
        }
    }
};

typedef TBox<float> Box;

}

#endif // BOX_HEADER
