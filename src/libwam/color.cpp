#include "color.h"
#include <sstream>

namespace wam
{
	Color Color::White	= Color(1,1,1,1);
	Color Color::Grey	= Color(0.5f, 0.5f, 0.5f, 1.0f);
	Color Color::Black	= Color(0,0,0,1);
	Color Color::Red	= Color(1,0,0,1);
	Color Color::Green	= Color(0,1,0,1);
	Color Color::Blue	= Color(0,0,1,1);
	Color Color::Yellow = Color(1,1,0,1);
	Color Color::Orange = Color(1.0f,0.5f,0.0f,1.0f);
	Color Color::Purple = Color(0.5f, 0.0f, 1.0f, 1.0f);

	Color::Color(const Color &color)
		: r(color.r), g(color.g), b(color.b), a(color.a)
	{
	}

	Color::Color()
		: r(1), g(1), b(1), a(1)
	{ }

	Color::Color(float r, float g, float b, float a)
		: r(r), g(g), b(b), a(a)
	{ 	}

	Color::Color(float r, float g, float b)
		: r(r), g(g), b(b), a(1)
	{ 	}

	Color::Color(const std::string &hexString)
	{
		std::istringstream is(hexString);

		unsigned int hexcolor;
		is >> std::hex >> hexcolor;

		r = ( hexcolor >> 16 ) & 0xFF;
		g = ( hexcolor >> 8 ) & 0xFF;
		b = hexcolor & 0xFF;
		r /= 255.0f;
		g /= 255.0f;
		b /= 255.0f;
		a = 1.0f;
	}
	
	bool Color::operator==(const Color& rhs)
	{
		return ((r == rhs.r) && (g == rhs.g) && (b == rhs.b) && (a == rhs.a));
	}

	bool Color::operator!=(const Color& rhs)
	{
		return ((r != rhs.r) || (g != rhs.g) || (b != rhs.b) || (a != rhs.a));
	}

	void Color::Clamp()
	{
		for (unsigned int i = 0; i < 4; i++)
		{
			if ((*this)[i] > 1.0f)
				(*this)[i] = 1.0f;
			if ((*this)[i] < 0.0f)
				(*this)[i] = 0.0f;
		}
	}

	float& Color::operator[] (unsigned int i)
	{
		switch(i)
		{
		case 0: return r;
		case 1: return g;
		case 2: return b;
		default: return a;
		}
	}

	Color operator+(const Color& lhs, const Color& rhs)
	{
		return Color(lhs.r + rhs.r, lhs.g + rhs.g, lhs.b + rhs.b, lhs.a + rhs.a);
	}
	
	Color operator-(const Color& lhs, const Color& rhs)
	{
		return Color(lhs.r - rhs.r, lhs.g - rhs.g, lhs.b - rhs.b, lhs.a - rhs.a);
	}

	Color operator*(float lhs, const Color& rhs)		// left scalar multiplication
	{
		return Color(lhs * rhs.r, lhs * rhs.g, lhs * rhs.b, lhs * rhs.a);
	}
	
	Color operator*(const Color& lhs, float rhs)		// right scalar multiplication
	{
		return Color(lhs.r * rhs, lhs.g * rhs, lhs.b * rhs, lhs.a * rhs);
	}

	Color operator/(const Color& lhs, float rhs)
	{
		return Color(lhs.r / rhs, lhs.g / rhs, lhs.b / rhs, lhs.a / rhs);
	}

	Color & operator+=(Color & left, const Color &right) {
		left.r += right.r;
		left.g += right.g;
		left.b += right.b;
		left.a += right.a;
		return left;
	}
	Color& operator -=(Color& left, const Color& right){
		left.r -= right.r;
		left.g -= right.g;
		left.b -= right.b;
		left.a -= right.a;
		return left;
	}

	Color& operator *=(Color& left, const Color& right){
		left.r *= right.r;
		left.g *= right.g;
		left.b *= right.b;
		left.a *= right.a;
		return left;
	}

	Color& operator *=(Color& left, const float& right){
		left.r *= right;
		left.g *= right;
		left.b *= right;
		left.a *= right;
		return left;
	}
}
