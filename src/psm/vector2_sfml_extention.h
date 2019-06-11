#ifndef THOR_VECTORALGEBRA2D_HPP
#define THOR_VECTORALGEBRA2D_HPP

#include <SFML/System/Vector2.hpp>
#include <cmath>
#include <cassert>

///
template <typename T>
T dotProduct(const sf::Vector2<T>& lhs, const sf::Vector2<T>& rhs){
	return lhs.x * rhs.x + lhs.y * rhs.y;
}

/// @brief Returns the square of @a vector's length.
/// @details Suitable for comparisons, more efficient than length().
template <typename T>
T squaredLength(const sf::Vector2<T>& vector){
	return dotProduct(vector, vector);
}

template <typename T>
T length(const sf::Vector2<T>& vector){
	return std::sqrt(squaredLength(vector));
}

/// @brief Adapts @a vector so that its length is |@a newLength| after this operation.
/// @details If @a newLength is less than zero, the vector's direction changes.
/// @pre @a vector is no zero vector.
template <typename T>
void setLength(sf::Vector2<T>& vector, T newLength){
	assert(vector != sf::Vector2<T>());
	vector *= newLength / length(vector);
}

/// @brief Returns a vector with same direction as the argument, but with length 1.
/// @pre @a vector is no zero vector.
template <typename T>
sf::Vector2<T> unitVector(const sf::Vector2<T>& vector){
	assert(vector != sf::Vector2<T>());
	return vector / length(vector);
}

/// @brief Returns the polar angle.
/// @details The vector (1,0) corresponds 0 degrees, (0,1) corresponds 90 degrees.
/// @return Angle in degrees in the interval [-180,180].
/// @pre @a vector is no zero vector.
template <typename T>
T polarAngle(const sf::Vector2<T>& vector){
	assert(vector != sf::Vector2<T>());
	return std::atan2(vector.y, vector.x);
}

/// @brief Sets the polar angle of the specified vector.
/// @details The vector (1,0) corresponds 0 degrees, (0,1) corresponds 90 degrees.
template <typename T>
void setPolarAngle(sf::Vector2<T>& vector, T newAngle){
	// No assert here, because turning a zero vector is well-defined (yields always zero vector)

	T vecLength = length(vector);

	vector.x = vecLength * std::cos(newAngle);
	vector.y = vecLength * std::sin(newAngle);
}

/// @brief Rotates the vector by the given angle (in degrees).
/// @details The vector (1,0) corresponds 0 degrees, (0,1) corresponds 90 degrees.
template <typename T>
void rotate(sf::Vector2<T>& vector, T angle){
	// No assert here, because turning a zero vector is well-defined (yields always zero vector)

	T cos = std::cos(angle);
	T sin = std::sin(angle);

	// Don't manipulate x and y separately, otherwise they're overwritten too early
	vector = sf::Vector2<T>(
		cos * vector.x - sin * vector.y,
		sin * vector.x + cos * vector.y);
}

/// @brief Returns a copy of the vector, rotated by @a angle degrees.
/// @details The vector (1,0) corresponds 0 degrees, (0,1) corresponds 90 degrees.
template <typename T>
sf::Vector2<T> rotatedVector(const sf::Vector2<T>& vector, T angle){
	// No assert here, because turning a zero vector is well-defined (yields always zero vector)

	sf::Vector2<T> copy = vector;
	rotate(copy, angle);
	return copy;
}

/// @brief Returns a perpendicular vector.
/// @details Returns @a vector turned by 90 degrees counter clockwise; (x,y) becomes (-y,x).
///  For example, the vector (1,0) is transformed to (0,1).
template <typename T>
sf::Vector2<T> perpendicularVector(const sf::Vector2<T>& vector){
	return sf::Vector2<T>(-vector.y, vector.x);
}

// ---------------------------------------------------------------------------------------------------------------------------

/// @brief Computes the dot product of two 2D vectors.


/// @brief Computes the cross product of two 2D vectors (Z component only).
/// @details Treats the operands as 3D vectors, computes their cross product and returns the result's Z component
///  (X and Y components are always zero).
template <typename T>
T crossProduct(const sf::Vector2<T>& lhs, const sf::Vector2<T>& rhs){
	return lhs.x * rhs.y - lhs.y * rhs.x;
}

/// @brief Computes the signed angle from @a lhs to @a rhs.
/// @return Angle in degrees in the interval [-180,180]. The angle determines how much you have to turn @a lhs
///  until it points to the same direction as @a rhs.
/// @pre Neither @a lhs nor @a rhs is a zero vector.
template <typename T>
T signedAngle(const sf::Vector2<T>& lhs, const sf::Vector2<T>& rhs){
	assert(lhs != sf::Vector2<T>() && rhs != sf::Vector2<T>());
	return std::atan2(crossProduct(lhs, rhs), dotProduct(lhs, rhs));
}

/// @brief Returns the component-wise product of @a lhs and @a rhs.
/// @details Computes <i>(lhs.x*rhs.x, lhs.y*rhs.y)</i>. Component-wise multiplications are mainly used for scales.
template <typename T>
sf::Vector2<T> cwiseProduct(const sf::Vector2<T>& lhs, const sf::Vector2<T>& rhs){
	return sf::Vector2<T>(lhs.x * rhs.x, lhs.y * rhs.y);
}

/// @brief Returns the component-wise quotient of @a lhs and @a rhs.
/// @details Computes <i>(lhs.x/rhs.x, lhs.y/rhs.y)</i>. Component-wise divisions are mainly used for scales.
/// @pre Neither component of @a rhs is zero.
template <typename T>
sf::Vector2<T> cwiseQuotient(const sf::Vector2<T>& lhs, const sf::Vector2<T>& rhs){
	assert(rhs.x != 0 && rhs.y != 0);
	return sf::Vector2<T>(lhs.x / rhs.x, lhs.y / rhs.y);
}

/// @brief Returns the projection of @a vector onto @a axis.
/// @param vector Vector to project onto another.
/// @param axis Vector being projected onto. Need not be a unit vector, but may not have length zero.
template <typename T>
sf::Vector2<T> projectedVector(const sf::Vector2<T>& vector, const sf::Vector2<T>& axis){
	assert(axis != sf::Vector2<T>());
	return dotProduct(vector, axis) / squaredLength(axis) * axis;
}

#endif