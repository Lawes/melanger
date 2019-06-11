#include "input.h"
#include <cstring>
#include <iostream>

#include <SFML/Graphics.hpp>

using namespace std;

CInput::CInput(sf::RenderWindow& w) :
	m_win(w),
	m_inputs{0},
	quit(false)
{
	auto size(m_win.getSize());
	sf::Vector2f v(m_win.mapPixelToCoords(sf::Vector2i(size.x, size.y)));
	m_invscaleX = v.x/size.x;
	m_invscaleY = v.y/size.y;
}

void CInput::unKey(int i) {
    m_inputs[i]=0;
}

void CInput::unMouseButton(int i) {
    m_inputs[sf::Keyboard::KeyCount+i] = 0;
}

bool CInput::update()
{
	sf::Event event;
	sf::Vector2f v;
    while (m_win.pollEvent(event))	{
		switch (event.type)
		{
		case sf::Event::KeyPressed:
			m_inputs[event.key.code]=1;
			lastkey=event.key.code;
			break;
		case sf::Event::KeyReleased:
			m_inputs[event.key.code]=0;
			break;
		case sf::Event::MouseMoved:
			v = m_win.mapPixelToCoords(sf::Vector2i(
				event.mouseMove.x,
				event.mouseMove.y));
			mousex = v.x;
			mousey = v.y;
			//mousex=event.mouseMove.x*m_invscaleX;
			//mousey=event.mouseMove.y*m_invscaleY;
			break;
		case sf::Event::MouseButtonPressed:
			m_inputs[sf::Keyboard::KeyCount+event.mouseButton.button]=1;
			break;
		case sf::Event::MouseButtonReleased:
			m_inputs[sf::Keyboard::KeyCount+event.mouseButton.button]=0;
			break;
		case sf::Event::Closed:
			quit = true;
			break;
		default:
			break;
		}
	}
	return quit;
}

bool CInput::Key(int i) {
	if( !m_inputs[i] ) return false;
//	else key[i]=0;
	return true;
}

