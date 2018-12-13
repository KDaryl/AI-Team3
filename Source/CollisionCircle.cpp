#include "CollisionCircle.h"

CollisionCircle::CollisionCircle() :
	m_position(0,0),
	m_r(0)
{
}

CollisionCircle::CollisionCircle(float x, float y, float r) :
	m_position(x, y),
	m_r(r)
{

}

CollisionCircle::~CollisionCircle()
{
}

bool CollisionCircle::intersects(CollisionCircle & other)
{
	auto one = powf(((other.x() + other.r()) - (m_position.x + m_r)), 2);
	auto two = powf(((m_position.y + m_r) - (other.y() + other.r())), 2);
	auto three = powf((m_r + other.r()), 2);

	if(one + two <= three)
		return true;
	return false;
}

void CollisionCircle::setRadius(float r)
{
	m_r = r;
}

void CollisionCircle::setSize(float x, float y, float r)
{
	m_position = Vector2f(x, y);
	m_r = r;
}

void CollisionCircle::setPosition(float x, float y)
{
	m_position = Vector2f(x, y);
}

void CollisionCircle::setPosition(Vector2f pos)
{
	m_position = pos;
}
