#pragma once
#include "Vector2f.h"

class CollisionCircle
{
public:
	CollisionCircle();
	CollisionCircle(float x, float y, float r);
	~CollisionCircle();

	void setRadius(float r);
	void setSize(float x, float y, float r);
	void setPosition(float x, float y);
	void setPosition(Vector2f pos);

	//Public variables
	Vector2f position;
	float r;
};

