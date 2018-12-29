#pragma once
#include "SFML/Graphics.hpp"
#include "InputHandler.h" //For input handling
#include "Thor/Animations.hpp" //For animations
#include "Thor/Math.hpp" //For math calculations
#include "ResourceManager.h" //For texture loading

//For physics, we include these for every physics object
#include "PhysicsHandler.h" 
using namespace physics;

class Player
{
public:
	Player();
	~Player() {}

	void update(double dt);
	void draw(sf::RenderWindow& win);
	void handleInput(InputHandler& input);
	void setTexture(ResourceManager& resources);

	//Public variables
	Vector2f m_position;
	Vector2f m_velocity;
	sf::FloatRect m_rangeCollider;
	sf::Sprite m_sprite;
	PhysicsBody m_physicsBody;
private:
	//Private methods
	void setupAnimations();

	Vector2f m_turnVector;
	float m_moveSpeed, m_turnSpeed, m_friction, m_angle, m_dt;

	//Sprite and animation variables
	thor::Animator<sf::Sprite, std::string> m_animator; //Animator

	bool m_isMoving, m_turningLeft, m_turningRight;
};
