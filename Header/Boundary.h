#pragma once
#include "Player.h"
#include "SFML/Graphics.hpp"
#include "Vector2f.h"

class Boundary
{
public:
	Boundary(float x, float y, float w, float h, Player& player);
	~Boundary();

	void update(double dt);
	void draw(sf::RenderWindow& win);

	sf::FloatRect m_collider;
private:
	Player * m_playerPtr;
	sf::RectangleShape m_debugRect;
};

