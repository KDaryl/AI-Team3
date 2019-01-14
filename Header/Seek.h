#pragma once
#include "AI.h"

class Seek : public AI
{
public:
	Seek(Vector2f position, float maxSpeed);
	~Seek();
	void update(Vector2f t);
	void render(sf::RenderWindow& window);
	
};

