#ifndef VECTOR2_HEADER
#define VECTOR2_HEADER

#include <iostream>

namespace wam {
	class Vector2
	{
	public:
		float x,y;

		static Vector2 zero;
		static Vector2 one;
		static Vector2 down;
		static Vector2 up;
		static Vector2 left;
		static Vector2 right;

		Vector2(const Vector2 &vector2);
		Vector2();
		Vector2(float x, float y);

		void set(float x, float y);

		// return random vector
		static Vector2 Random();
		static Vector2 Reflect(const Vector2& a, const Vector2& b);

		float sqNorm() const;
		float norm() const;
		Vector2 getNormalized() const;
		float normalize();
		void orthogonal();
		Vector2 getPerpendicularLeft() const;
		Vector2 getPerpendicularRight() const;
		void clamp(float max);
		bool isInRange(float range) const;
		float dot(const Vector2& other) const;
		float cross(const Vector2& other) const;
		Vector2 Reflect(const Vector2 &other);
		//! return angle in radians
		float GetAngleRadians();
		float GetAngleDegrees();
		//! initialize x and y from the passed angle in degrees
		void setFromAngleDegrees(float angle);

		/*
		static Vector2 Add(Vector2 a, Vector2 b);
		static Vector2 Subtract(Vector2 a, Vector2 b);
		static Vector2 Multiply(Vector2 a, float s);
		static Vector2 Scale(Vector2 a, float s);
		*/

		Vector2 xx() const;
		Vector2 yy() const;
		Vector2 yx() const;

		float& operator[] (unsigned int i);

		Vector2& operator=(const Vector2& rhs);
		bool operator==(const Vector2& rhs);
		bool operator!=(const Vector2& rhs);

		Vector2& operator+=(const Vector2& rhs);
		Vector2& operator-=(const Vector2& rhs);
		Vector2& operator*=(float rhs);				// scalar multiplication
		Vector2& operator/=(float rhs);				// scalar inverse multiplication
	};

	Vector2 operator+(const Vector2& lhs, const Vector2& rhs);
	Vector2 operator+(float lhs, const Vector2& rhs);
	Vector2 operator+(const Vector2& lhs, float rhs);

	Vector2 operator-(const Vector2& lhs, const Vector2& rhs);
	Vector2 operator-(float lhs, const Vector2& rhs);
	Vector2 operator-(const Vector2& lhs, float rhs);

	Vector2 operator-(const Vector2 &rhs);

	Vector2 operator*(float lhs, const Vector2& rhs);		// left scalar multiplication
	Vector2 operator*(const Vector2& lhs, float rhs);		// right scalar multiplication
	Vector2 operator/(const Vector2& lhs, float rhs);		// right scalar inverse multiplication
	Vector2 operator/(float lhs, const Vector2 &rhs);

	Vector2 operator*(const Vector2& lhs, const Vector2& rhs);	// multiply components (scale)
	//float operator^(const Vector2& lhs, const Vector2& rhs);	// cross product
}

std::ostream& operator<<(std::ostream& out, const wam::Vector2& vec); // output

#endif