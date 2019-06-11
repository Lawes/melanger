#ifndef INPUT_HEADER
#define INPUT_HEADER

#include <SFML/Graphics.hpp>

class CInput
{
protected:
	sf::RenderWindow& m_win;
	char m_inputs[sf::Keyboard::KeyCount+sf::Mouse::ButtonCount];
	float mousex,mousey;
	float mousexrel,mouseyrel;
	float m_invscaleX, m_invscaleY;
	bool quit;
	int lastkey;

public:
	CInput(sf::RenderWindow& w);
	~CInput(){}
	bool update();
	bool Key(int i);
	void unKey(int i);
	void unMouseButton(int i);
	inline int lastKey() { return lastkey; }
	inline float MouseX() const {return mousex;}
	inline float MouseY()const {return mousey;}
	inline float MouseXrel() const {return mousexrel;}
	inline float MouseYrel() const {return mouseyrel;}
	inline int MouseButton(int i) const {return m_inputs[sf::Keyboard::KeyCount+i];}
	inline bool Quit() const {return quit;}
};
#endif
