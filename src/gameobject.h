#ifndef GAMEOBJECT_HEADER
#define GAMEOBJECT_HEADER

#include <SFML/Graphics.hpp>

#define INTERFACE_GameObject(terminal) \
        public: \
            virtual void start() terminal \
            virtual void stop() terminal \
            virtual void update(float dt) terminal \
            virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const terminal

//#define BASE_GameObject INTERFACE_GameObject(=0;)
#define BASE_GameObject INTERFACE_GameObject({};)
#define DERIVED_GameObject INTERFACE_GameObject(;)


class GameObject : public sf::Drawable {

    public:
        GameObject() {};
        virtual ~GameObject() {};
    BASE_GameObject
};

#endif