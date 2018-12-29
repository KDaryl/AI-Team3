#include "CollisionBox.h"

CollisionBox::CollisionBox() :
	position(0, 0),
	w(0),
	h(0),
	min(0, 0),
	max(0, 0)
{
}

CollisionBox::CollisionBox(float x, float y, float w, float h) :
	position(x, y),
	w(w),
	h(h),
	min(position),
	max(position.x + w, position.y + h)
{
}

CollisionBox::~CollisionBox()
{
}

void CollisionBox::setSize(float w, float h)
{
	w = w;
	h = h;
}

void CollisionBox::setSize(float x, float y, float w, float h)
{
	position.x = x;
	position.y = y;
	w = w;
	h = h;
}

void CollisionBox::setPosition(float x, float y)
{
	position = Vector2f(x, y); //Set the position
	min = position; //Set teh min position
	max = Vector2f(position.x + w, position.y + h); //Set the max
}

void CollisionBox::setPosition(Vector2f pos)
{
	position = pos;
	min = pos; //Set teh min position
	max = Vector2f(pos.x + w, pos.y + h); //Set the max
}
