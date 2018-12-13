#pragma once
#include "Vector2f.h"

class CollisionBox
{
public:
	CollisionBox();
	CollisionBox(float x, float y, float w, float h);
	~CollisionBox();

	bool intersects(CollisionBox& other);
	void setSize(float w, float h);
	void setSize(float x, float y, float w, float h);
	void setPosition(float x, float y);
	void setPosition(Vector2f pos);

	//Getters
	float x() { return m_position.x; }
	float y() { return m_position.y; }
	float w() { return m_w; }
	float h() { return m_h; }
	Vector2f getPosition() { return m_position; }
private:
	float m_w, m_h; //Width and height of the collision box
	Vector2f m_position; //Position of the collision box
};

