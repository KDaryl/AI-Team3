#include "Vector2f.h"

Vector2f::Vector2f() :
	x(0),
	y(0)
{
}

Vector2f::Vector2f(float _x, float _y) :
	x(_x),
	y(_y)
{
}

Vector2f::~Vector2f()
{
}

void Vector2f::operator=(const Vector2f & v)
{
	x = v.x;
	y = v.y;
}

void Vector2f::operator=(const sf::Vector2i & v)
{
	x = v.x;
	y = v.y;
}

void Vector2f::operator=(const sf::Vector2f & v)
{
	x = v.x;
	y = v.y;
}

bool Vector2f::operator==(const Vector2f & v)
{
	return (x == v.x && y == v.y) ? true : false;
}

bool Vector2f::operator!=(const Vector2f & v)
{
	return (x == v.x && y == v.y) ? false : true;
}

Vector2f Vector2f::operator/(const float val)
{
	return Vector2f(x / val, y / val);
}

Vector2f Vector2f::operator+(const Vector2f & v)
{
	return Vector2f(x + v.x, y + v.y);
}

void Vector2f::operator+=(const Vector2f & v)
{
	//Add values to our x and y
	x += v.x;
	y += v.y;
}

void Vector2f::operator-=(const Vector2f & v)
{
	x -= v.x;
	y -= v.y;
}

void Vector2f::operator/=(const Vector2f & v)
{
	x /= v.x;
	y /= v.y;
}

void Vector2f::operator*=(float val)
{
	x *= val;
	y *= val;
}

Vector2f Vector2f::operator-(const Vector2f & v)
{
	return Vector2f(x - v.x, y - v.y);
}

Vector2f Vector2f::operator*(float val)
{
	return Vector2f(x * val, y * val);
}

std::ostream & operator<<(std::ostream & out, const Vector2f & v)
{
	out << "(" << std::to_string(v.x);
	out << ", " << std::to_string(v.y) << ")";
	return out;
}

float Vector2f::distance(Vector2f & v1, Vector2f & v2)
{
	auto minus = v1 - v2;
	return minus.magnitude();
}

float Vector2f::magnitude()
{
	return sqrtf(powf(x, 2) + powf(y, 2));;
}

float Vector2f::sqrdMagnitude()
{
	return powf(x, 2) + powf(y, 2);
}

float Vector2f::dotProduct(Vector2f v)
{
	return (x * v.x) + (y * v.y);
}

Vector2f Vector2f::normalise()
{
	float len = this->magnitude();
	return Vector2f(x / len, y / len);
}

void Vector2f::zeroVector()
{
	x = 0;
	y = 0;
}