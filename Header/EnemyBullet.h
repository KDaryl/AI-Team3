#pragma once
#include "AI.h"

class EnemyBullet
{
public:
	EnemyBullet();

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