#include "vector2.h"
#include "random.h"
#include <cmath>

namespace wam {
	Vector2 Vector2::zero = Vector2(0,0);
	Vector2 Vector2::one = Vector2(1,1);
	Vector2 Vector2::up = Vector2(0, -1); //2d style
	Vector2 Vector2::down = Vector2(0, 1);
	Vector2 Vector2::left = Vector2(-1, 0);
	Vector2 Vector2::right = Vector2(1, 0);

	Vector2::Vector2(const Vector2 &vector2)
		: x(vector2.x), y(vector2.y)
	{ }

	Vector2::Vector2()
		: x(0.0f), y(0.0f)
	{ }

	Vector2::Vector2(float x, float y)
		: x(x), y(y)
	{ }

    void Vector2::set(float x, float y)
    {
        this->x = x, this->y = y;
    }

	Vector2 Vector2::Random()
	{
		float rad = (Random::Int(0,360)) * wam::deg2Rad;
		//x = sin(rad);
		//y = cos(rad);
		return Vector2(sin(rad), cos(rad));
	}

	float Vector2::sqNorm() const
	{
		return pow(x, 2) + pow(y, 2);
	}

	float Vector2::norm() const
	{
		return sqrt(pow(x, 2) + pow(y, 2));
	}

	Vector2 Vector2::getNormalized() const
	{
		float invMag = 1.0/norm();
		return Vector2(x * invMag, y * invMag);
	}

	float Vector2::normalize()
	{
	    float n = norm();
		float invMag = 1.0/n;
		x *= invMag;
		y *= invMag;
		return n;
	}

    void Vector2::orthogonal() {
        float tmp = x;
        x = -y;
        y = tmp;
    }

	Vector2 Vector2::getPerpendicularLeft() const
	{
		return Vector2(-y, x);
	}

	Vector2 Vector2::getPerpendicularRight() const
	{
		return Vector2(y, -x);
	}

	void Vector2::setFromAngleDegrees(float angle)
	{
		x = sin(angle * wam::deg2Rad);
		y = cos(angle * wam::deg2Rad);
	}

	void Vector2::clamp(float max)
	{
		if ((pow(x, 2) + pow(y, 2)) > pow(max, 2))
		{
			normalize();
			x *= max;
			y *= max;
		}
	}

	bool Vector2::isInRange(float range) const
	{
		return ((pow(x, 2) + pow(y, 2)) <= pow(range, 2));
	}

	float Vector2::dot(const Vector2& b) const
	{
		return (x * b.x + y * b.y);
	}

	float Vector2::cross(const Vector2& b) const
	{
		return (x * b.y - y * b.x);
	}

	// static version
	Vector2 Vector2::Reflect(const Vector2& a, const Vector2& b)
	{
		Vector2 newVec;
		float dotProduct = -a.x*b.x - a.y*b.y;
		newVec.x = a.x + 2 * b.x * dotProduct;
		newVec.y = a.y + 2 * b.y * dotProduct;
		return newVec;
	}

	Vector2 Vector2::Reflect(const Vector2 &other)
	{
		float dotProduct = -x*other.x - y*other.y;
		x = x + 2 * other.x * dotProduct;
		y = y + 2 * other.y * dotProduct;
		return *this;
	}

	float Vector2::GetAngleRadians()
	{
		float angle = atan2(y, x);
		if (angle < 0)
		{
			angle += 2 * wam::pi;
		}
		return angle;
	}

	float Vector2::GetAngleDegrees()
	{
		return GetAngleRadians() * wam::rad2Deg;
	}

	Vector2 Vector2::xx()const
	{
		return Vector2(this->x, this->x);
	}

	Vector2 Vector2::yy()const
	{
		return Vector2(this->y, this->y);
	}

	Vector2 Vector2::yx()const
	{
		return Vector2(this->y, this->x);
	}

	float& Vector2::operator[] (unsigned int i)
	{
		switch(i)
		{
		case 0: return x;
		case 1: return y;

		default: return x; // TODO: Handle this scenario with an assertion?
		}
	}

	Vector2& Vector2::operator=(const Vector2& rhs)
	{
		if (&rhs != this)
		{
			x = rhs.x;
			y = rhs.y;
		}
		return *this;
	}

	bool Vector2::operator==(const Vector2& rhs)
	{
		return ((x == rhs.x) && (y == rhs.y));
	}

	bool Vector2::operator!=(const Vector2& rhs)
	{
		return ((x != rhs.x) || (y != rhs.y));
	}

	Vector2& Vector2::operator+=(const Vector2& rhs)
	{
		x = x + rhs.x;
		y = y + rhs.y;
		return *this;
	}

	Vector2& Vector2::operator-=(const Vector2& rhs)
	{
		x = x - rhs.x;
		y = y - rhs.y;
		return *this;
	}

	Vector2& Vector2::operator*=(float rhs)				// scalar multiplication
	{
		x = x * rhs;
		y = y * rhs;
		return *this;
	}

	Vector2& Vector2::operator/=(float rhs)				// scalar inverse multiplication
	{
		x = x / rhs;
		y = y / rhs;
		return *this;
	}

	Vector2 operator+(const Vector2& lhs, const Vector2& rhs)
	{
		return Vector2(lhs.x + rhs.x, lhs.y + rhs.y);
	}

	Vector2 operator+(float lhs, const Vector2& rhs)
	{
		return Vector2(lhs + rhs.x, lhs + rhs.y);
	}

	Vector2 operator+(const Vector2& lhs, float rhs)
	{
		return Vector2(lhs.x + rhs, lhs.y + rhs);
	}

	Vector2 operator-(const Vector2& lhs, const Vector2& rhs)
	{
		return Vector2(lhs.x - rhs.x, lhs.y - rhs.y);
	}
	Vector2 operator-(float lhs, const Vector2& rhs)
	{
		return Vector2(lhs - rhs.x, lhs - rhs.y);
	}
	Vector2 operator-(const Vector2& lhs, float rhs)
	{
		return Vector2(lhs.x - rhs, lhs.y - rhs);
	}

	Vector2 operator-(const Vector2 &rhs)
	{
		return Vector2(-rhs.x, -rhs.y);
	}

	Vector2 operator*(float lhs, const Vector2& rhs)		// left scalar multiplication
	{
		return Vector2(lhs * rhs.x, lhs * rhs.y);
	}

	Vector2 operator*(const Vector2& lhs, float rhs)		// right scalar multiplication
	{
		return Vector2(lhs.x * rhs, lhs.y * rhs);
	}

	Vector2 operator/(const Vector2& lhs, float rhs)		// right scalar inverse multiplication
	{
		return Vector2(lhs.x / rhs, lhs.y / rhs);
	}

	Vector2 operator*(const Vector2& lhs, const Vector2& rhs)		// multiply component (scale)
	{
		return Vector2(lhs.x * rhs.x, lhs.y * rhs.y);
	}

	Vector2 operator/(float lhs, const Vector2 &rhs)
	{
		return Vector2(lhs / rhs.x, lhs / rhs.y);
	}

	/*
	float operator^(const Vector2& lhs, const Vector2& rhs)		// cross product
	{
		return lhs.x * rhs.y - lhs.y * rhs.x;
	}
	*/
}

std::ostream& operator<<(std::ostream& out, const wam::Vector2& vec)
{
    out << '(' << vec.x << ',' << vec.y << ')';
    return out;
}