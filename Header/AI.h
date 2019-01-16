#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "ResourceManager.h"
#include "Vector2f.h"
#include "Thor/Math.hpp"
#include "Grid.h" //For path finding
#include "Thor/Animations.hpp"
#include <random>
#include "Player.h"

//For physics, we include these for every physics object
#include "PhysicsHandler.h"
//using namespace physics;

class AI
{
public:
	AI();
	~AI();

	//pure virtual functions
  virtual void update(float dt) = 0;
	virtual void render(sf::RenderWindow& window) = 0;

	static Vector2f truncate(Vector2f v, float max);

	//getters
	sf::Sprite& getSprite();
	Vector2f getVelocity();
	float getNewOrientationByVelocity(float currentOrientation, Vector2f currentVelocity);
	float getNewOrientationByPosition(float currentOrientation, Vector2f currentVelocity);
	static float getOrientation(float currentAngle, Vector2f vel);


	//setters
	void setTexture(ResourceManager& resources, std::string name, Vector2f center);
	void setTarget(Vector2f target);
	void setAngle(Vector2f& v, float f);

	//types
	void seek();
	void flee();
	void wander();
	void swarm();
	void flock();

protected:
	//player pointer
	Player* m_player;

	//draw ship
	sf::Sprite m_sprite;
	sf::Texture m_texture;

	//vectors
	Vector2f m_position;
	Vector2f m_velocity;
	Vector2f m_target;
	Vector2f m_desiredVelocity;
	Vector2f m_steering;
	sf::FloatRect m_rangeCollider;

	//collisions
	PhysicsBody m_physicsBody;

	float m_maxSpeed;
	float m_orientation;
	float m_mass;

	//wander variables
	Vector2f m_circleCenter;
	Vector2f m_wanderForce;
	Vector2f m_displacement;
	float m_wanderAngle = 0;
	float m_randomNum  = 0;
	const float CIRCLE_DISTANCE = 100.0f;
	const float CIRCLE_RADIUS = 25.0f;
	const float ANGLE_CHANGE = 1.0f;
};
