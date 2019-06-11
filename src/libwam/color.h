#ifndef COLOR_HEADER
#define COLOR_HEADER

#include <string>

namespace wam
{
	//! Describes a color using 4 floats in 0.0 - 1.0 range.
	class Color
	{
	public:
		Color(const Color &color);
		Color();
		Color(float r, float g, float b);
		Color(float r, float g, float b, float a);
		Color(const std::string &hexString);

        //! Clamps the rgba values to between 0 and 1.0, inclusive.
		void Clamp();

        //! Indirect accessor for rgba values. Color[0] = r, [1] = g, [2] = b, [3] = a, [4+] = undefined
		float& operator[] (unsigned int i);
		bool operator==(const Color& rhs);
		bool operator!=(const Color& rhs);

		//! Red, green, blue, alpha (translucency)
		float r, g, b, a;

		static Color White;
		static Color Grey;
		static Color Black;
		static Color Red;
		static Color Green;
		static Color Blue;
		static Color Yellow;
		static Color Orange;
		static Color Purple;
	};

	Color operator+(const Color& lhs, const Color& rhs);	
	Color operator-(const Color& lhs, const Color& rhs);
	Color operator*(float lhs, const Color& rhs);		// left scalar multiplication
	Color operator*(const Color& lhs, float rhs);		// right scalar multiplication
	Color operator/(const Color& lhs, float rhs);
	Color & operator+=(Color & left, const Color &right);
	Color& operator -=(Color& left, const Color& right);
	Color& operator *=(Color& left, const Color& right);
	Color& operator *=(Color& left, const float& right);
}

#endif
