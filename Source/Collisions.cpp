#include "Collisions.h"

Collisions::Collisions()
{
}

Collisions::~Collisions()
{
}

bool Collisions::AABBvsAABB(CollisionBox a, CollisionBox b)
{
	// Exit with no intersection if found separated along an axis
	if (a.max.x < b.min.x or a.min.x > b.max.x) return false; 
	if (a.max.y < b.min.y or a.min.y > b.max.y) return false;

	// No separating axis found, therefor there is at least one overlapping axis
	return true;
}

bool Collisions::CircleVsCircle(CollisionCircle a, CollisionCircle b)
{
	float r = a.r + b.r;
	r *= r;
	return r < std::powf(a.position.x + b.position.x, 2) + std::powf(a.position.y + b.position.y, 2);
}
