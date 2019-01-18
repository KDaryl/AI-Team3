#pragma once
#include "SFML/Graphics.hpp"
#include "Thor/Animations.hpp"
#include "ResourceManager.h"
#include "Player.h"

/**
* Description: simple door class that openswhen a player is nearby
*/

class Door
{
public:
	Door(float x, float y, Player& player);
	~Door();

	void update(double dt);
	void draw(sf::RenderWindow& win);
	void setTexture(ResourceManager& resources);

	//Setters
	void setRotation(float angle);

	//Getters
	sf::FloatRect collider() { return m_sprite.getGlobalBounds(); }
	CollisionCircle m_rangeCollider;
private:
	bool m_closing;
	Vector2f m_position;
	Player* m_playerPtr;

	sf::Sprite m_sprite;
	thor::Animator<sf::Sprite, std::string> m_animator;
};

