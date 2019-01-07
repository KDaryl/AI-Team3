#pragma once
#include "Vector2f.h"

class CollisionBox
{
public:
	CollisionBox();
	CollisionBox(float x, float y, float w, float h);
	~CollisionBox();

	void setBox();
	void rotate(float angle);
	void setSize(float w, float h);
	void setSize(float x, float y, float w, float h);
	void setPosition(float x, float y);
	void setPosition(Vector2f pos);
	void draw(sf::RenderWindow& window);

	
	//Public variables
	Vector2f position, min, max;
	float w, h; //Width and height of the collision box
	sf::RectangleShape rect; //Rectangle to represent the collider
};

