#include "Wander.h"



Wander::Wander(Vector2f position, float maxSpeed)
{
	m_velocity = Vector2f(0, 0);
	m_target = Vector2f(0, 0);
	m_desiredVelocity = Vector2f(0, 0);
	m_steering = Vector2f(0, 0);
	m_position = position;
	m_maxSpeed = maxSpeed;
	m_orientation = 0;

	//Set our box parameters to our position and size and to NOT use gravity
	//m_physicsBody.setCircleParameters(Vector2f(m_position.x, m_position.y), 5, 1.25f, false);
	//m_physicsBody.setFriction(0.1);
	//m_physicsBody.setRestitution(0.1f);

	////Add our body to our physics world
	//physics::world->addPhysicsBody(m_physicsBody);
}


Wander::~Wander()
{
}

Vector2f Wander::calculateWander()
{
	// Calculate the circle center
	m_circleCenter = m_velocity;

	m_circleCenter = vectorNormalise(m_circleCenter);
	m_circleCenter *= CIRCLE_DISTANCE;
	
	// Randomly change the vector direction
	// by making it change its current angle
	m_displacement = sf::Vector2f(0, -1);
	// Calculate the displacement force
	m_displacement *= CIRCLE_RADIUS;

	setAngle(m_displacement, m_wanderAngle);
	//
	// Change wanderAngle just a bit, so it
	// won't have the same value in the
	// next game frame.
	m_randomNum = rand() % 10;
	m_wanderAngle += (((m_randomNum / 10) + 0.05)  * ANGLE_CHANGE) - (ANGLE_CHANGE * .5);
	//std::cout << m_wanderAngle << std::endl;
	// Finally calculate and return the wander force

	m_wanderForce = m_circleCenter + m_displacement;
	return m_wanderForce;
}

void Wander::update(Vector2f t)
{
	// Calculate the wander force#
	m_steering = calculateWander();
	m_steering = truncate(m_steering, 10.0f);
	m_velocity = truncate(m_velocity + m_steering, m_maxSpeed);
	m_position = m_position + m_velocity;

	m_sprite.setPosition(sf::Vector2f(m_position.x, m_position.y));
	m_orientation = getNewOrientationByVelocity(m_orientation, m_velocity);
	m_sprite.setRotation(m_orientation);
}

void Wander::render(sf::RenderWindow & window)
{
	window.draw(m_sprite);
}

