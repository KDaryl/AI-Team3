#pragma once
#include "SFML/Graphics.hpp"
#include "InputHandler.h" //For input handling
#include "Thor/Animations.hpp" //For animations
#include "Thor/Math.hpp" //For math calculations
#include "ResourceManager.h" //For texture loading
//For physics
#include "PhysicsHandler.h"

class PlayerBullet
{
public:
	PlayerBullet();

	void update(double dt);
	void draw(sf::RenderWindow& window);

	void spawn(Vector2f position, float angle);

	void setTexture(ResourceManager& resources);

	void hasCollided();

	bool alive;
	bool collided;
	bool dead;
private:
	Vector2f m_position, m_velocity;
	float m_angle, m_speed; //The angle and speed of the bullet

	PhysicsBody m_body;

	//Sprite and animation variables
	thor::Animator<sf::Sprite, std::string> m_animator; //Animator
	sf::Sprite m_sprite;
};