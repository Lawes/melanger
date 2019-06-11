#ifndef STATE_HEADER
#define STATE_HEADER

class Shape;

namespace VODZO {
    class Vector2;
}

class State {
    public:

        enum SensRotation {
            ROT_PLUS = 1,
            ROT_MOINS = -1
        };

        State() : id_(0), enDeplacement(false), enRotation(false) {
        }

        void init( int cible, int p, int rotation, const VODZO::Vector2& pos, int taille);
        void finDeplacement();
        void finRotation();

        bool good();

        void goPos(const int newPos);
        void goRotation(SensRotation rot);

        int getPos() const {
            return pos_;
        }

        int getRotation() const {
            return rot_*90;
        }

        void setShape(Shape *s) {
            shape_ = s;
        }

    private:
        Shape *shape_;
        int pos_, rot_;
        int id_;
        bool enDeplacement, enRotation;

};



#endif
