#pragma once
#include "Vector2f.h"

class CollisionBox
{
public:
	CollisionBox();
	CollisionBox(float x, float y, float w, float h);
	~CollisionBox();

	void setSize(float w, float h);
	void setSize(float x, float y, float w, float h);
	void setPosition(float x, float y);
	void setPosition(Vector2f pos);
	
	//Public variables
	Vector2f position, min, max;
	float w, h; //Width and height of the collision box
};

