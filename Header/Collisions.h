#pragma once
#include "CollisionCircle.h"
#include "CollisionBox.h"

class Collisions
{
public:
	Collisions();
	~Collisions();

	static bool AABBvsAABB(CollisionBox a, CollisionBox b);
	static bool CircleVsCircle(CollisionCircle a, CollisionCircle b);
};