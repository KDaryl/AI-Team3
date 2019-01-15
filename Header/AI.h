#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "ResourceManager.h"
#include "Vector2f.h"
#include "Thor/Math.hpp"

//For physics, we include these for every physics object
#include "PhysicsHandler.h"
//using namespace physics;

class AI
{
public:
	AI();
	~AI();

	static Vector2f truncate(Vector2f v, float max);
	void setAngle(Vector2f& v, float f);
	
	//pure virtual functions
	virtual void update(Vector2f t) = 0;
	virtual void render(sf::RenderWindow& window) = 0;

	//getters
	sf::Sprite& getSprite();
	Vector2f getVelocity();
	float getNewOrientationByVelocity(float currentOrientation, Vector2f currentVelocity);
	float getNewOrientationByPosition(float currentOrientation, Vector2f currentVelocity);
	static float getOrientation(float currentAngle, Vector2f vel);
	
	
	//setters
	void setTexture(ResourceManager& resources, std::string name, Vector2f center);

	//vector calculations
	float vectorLength(Vector2f v);
	Vector2f vectorNormalise(Vector2f v);

protected:
	sf::Sprite m_sprite;
	sf::Texture m_texture;
	
	//vectors
	Vector2f m_velocity;
	Vector2f m_position;
	Vector2f m_target;
	Vector2f m_desiredVelocity;
	Vector2f m_steering;
	sf::FloatRect m_rangeCollider;
	
	//collisions
	PhysicsBody m_physicsBody;

	float m_maxSpeed;
	float m_orientation;
	float m_maxOrientation = 1;
	float m_mass = 1;
	
};
