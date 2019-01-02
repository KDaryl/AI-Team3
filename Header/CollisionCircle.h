#pragma once
#include "Vector2f.h"

class CollisionCircle
{
public:
	CollisionCircle();
	CollisionCircle(float x, float y, float r);
	~CollisionCircle();

	void setCircle();
	void setRadius(float r);
	void setSize(float x, float y, float r);
	void setPosition(float x, float y);
	void setPosition(Vector2f pos);
	void draw(sf::RenderWindow& window);

	//Public variables
	Vector2f position;
	float r;
	sf::CircleShape circle; //Circle to represent the collider
};

