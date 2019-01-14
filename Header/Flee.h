#pragma once
#include "AI.h"

class Flee: public AI
{
public:
	Flee(Vector2f position, float maxSpeed);
	~Flee();
	void update(Vector2f t);
	void render(sf::RenderWindow& window);
};

