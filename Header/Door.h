#pragma once
#include "SFML/Graphics.hpp"
#include "Thor/Animations.hpp"
#include "ResourceManager.h"
#include "Player.h"

///
///Description: Simple door class that opens and closes when a player is in/out of range
///

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
	sf::FloatRect collider() { return m_collider; }
private:
	bool m_closing;
	Vector2f m_position;
	Player* m_playerPtr;

	sf::Sprite m_sprite;
	sf::FloatRect m_collider;
	thor::Animator<sf::Sprite, std::string> m_animator;
};

