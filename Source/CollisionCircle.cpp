#include "CollisionCircle.h"

CollisionCircle::CollisionCircle() :
	position(0,0),
	r(0)
{
}

CollisionCircle::CollisionCircle(float x, float y, float r) :
	position(x, y),
	r(r)
{

}

CollisionCircle::~CollisionCircle()
{
}

void CollisionCircle::setRadius(float r)
{
	r = r;
}

void CollisionCircle::setSize(float x, float y, float r)
{
	position = Vector2f(x, y);
	r = r;
}

void CollisionCircle::setPosition(float x, float y)
{
	position = Vector2f(x, y);
}

void CollisionCircle::setPosition(Vector2f pos)
{
	position = pos;
}
