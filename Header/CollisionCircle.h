#pragma once
#include "Vector2f.h"
class CollisionCircle
{
public:
	CollisionCircle();
	CollisionCircle(float x, float y, float r);
	~CollisionCircle();

	bool intersects(CollisionCircle& other);
	void setRadius(float r);
	void setSize(float x, float y, float r);
	void setPosition(float x, float y);
	void setPosition(Vector2f pos);

	//Getters
	Vector2f getPosition() { return m_position; }
	float x() { return m_position.x; }
	float y() { return m_position.y; }
	float r() { return m_r; }
private:
	Vector2f m_position; //Position of the circle
	float m_r; //Radius of the circle
};

