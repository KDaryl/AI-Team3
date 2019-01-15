#pragma once
#include "PlayerBullet.h"

class Player
{
public:
	Player(float x, float y);
	~Player() {}

	void update(double dt);
	void draw(sf::RenderWindow& win, float a);
	void handleInput(InputHandler& input);
	void setTexture(ResourceManager& resources);
	
	//Public variables
	Vector2f m_position, m_prevPos;
	Vector2f m_velocity;
	CollisionCircle m_rangeCollider;
	sf::Sprite m_sprite;
	PhysicsBody m_physicsBody;
private:
	//Private methods
	void setupAnimations();

	Vector2f m_turnVector;
	float m_moveSpeed, m_turnSpeed, m_friction, m_angle, m_dt, m_maxSpeed;

	//Player bullets, we pool them to avoid nunecessary creation/deletion
	std::vector<PlayerBullet*> m_bullets;

	//Sprite and animation variables
	thor::Animator<sf::Sprite, std::string> m_animator; //Animator

	bool m_isMoving, m_turningLeft, m_turningRight;
};
