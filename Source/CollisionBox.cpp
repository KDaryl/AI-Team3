#include "CollisionBox.h"

CollisionBox::CollisionBox() :
	m_position(0,0),
	m_w(0),
	m_h(0)
{
}

CollisionBox::CollisionBox(float x, float y, float w, float h) :
	m_position(x, y),
	m_w(w),
	m_h(h)
{
}

CollisionBox::~CollisionBox()
{
}

bool CollisionBox::intersects(CollisionBox & other)
{
	//Checks for rectangular collision
	if (m_position.x < other.x() + other.w() &&
		m_position.x + m_w > other.x() &&
		m_position.y < other.y() + other.h() &&
		m_h + m_position.y > other.y())
		return true;
	return false;
}

void CollisionBox::setSize(float w, float h)
{
	m_w = w;
	m_h = h;
}

void CollisionBox::setSize(float x, float y, float w, float h)
{
	m_position.x = x;
	m_position.y = y;
	m_w = w;
	m_h = h;
}

void CollisionBox::setPosition(float x, float y)
{
	m_position = Vector2f(x, y); //Set the position
}

void CollisionBox::setPosition(Vector2f pos)
{
	m_position = pos;
}
