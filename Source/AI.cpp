#define _USE_MATH_DEFINES
#include "AI.h"
#include <cmath> 

AI::AI():
	m_physicsBody(Type::Dynamic, Shape::Circle, this)
{
	
}


AI::~AI()
{
}
/**
* function to truncate a vector to some value between 0 and 1 
*/
Vector2f AI::truncate(Vector2f v, float max)
{
	float i = 0;
	i = max / v.magnitude();
	i = i < 1.0 ? i : 1.0;
	v *= i;
	return v;
}
/**
* function used to get the orientation using the sprites position 
*/
float AI::getNewOrientationByPosition(float currentOrientation, Vector2f currentVelocity)
{
	// Note atan2 returns an angle in radians which you 
	// may want to convert to degrees.
	if (currentVelocity.magnitude() > 0)
	{
		const auto radAngle = atan2(-m_sprite.getPosition().y, m_sprite.getPosition().x);
		return (radAngle)* (180 / M_PI);
	}
	else
	{
		return currentOrientation;
	}
}
/**
*function used to get the orientation using the sprites velocity  
*/
float AI::getOrientation(float currentAngle, Vector2f vel)
{
	return vel.magnitude() > 0 ? atan2(vel.y, vel.x) * 57.2958 : currentAngle;
}
/**
*function used to get the orientation using the sprites velocity
*/
float AI::getNewOrientationByVelocity(float currentOrientation, Vector2f currentVelocity)
{
	// Note atan2 returns an angle in radians which you 
	// may want to convert to degrees.
	if (currentVelocity.magnitude() > 0)
	{
		return (atan2(-currentVelocity.x, currentVelocity.y))* (180.0f / M_PI) + 180;
	}
	else
	{
		return currentOrientation;
	}
}
/**
* function used to set the angle of vector 
*/
void AI::setAngle(Vector2f & v, float f)
{
	float len = v.magnitude();
	v.x = cos(f * (M_PI / 180.0f)) * len;
	v.y = sin(f* (M_PI / 180.0f)) * len;
}
/**
*function used to return the sprite 
*/
sf::Sprite & AI::getSprite()
{
	return m_sprite;
}
/**
*function used to return the velocity 
*/
Vector2f AI::getVelocity()
{
	return m_velocity;
}
/**
*function used to set the texture of the sprite
*/
void AI::setTexture(ResourceManager & resources, std::string name, Vector2f center)
{
	m_sprite.setTexture(resources.getTexture(name));
	m_sprite.setPosition(sf::Vector2f(m_position.x, m_position.y));
	//Set center of sprite
	m_sprite.setOrigin(sf::Vector2f(center.x,center.y));
}
/**
*function used to set the target the ai uses for calculations 
*/
void AI::setTarget(Vector2f target)
{
	m_target = target;
}
/**
*function used to calculate the seek behaviour 
*/
void AI::seek()
{
	m_velocity = m_velocity.normalise();
	m_velocity = sf::Vector2f(m_velocity.x * m_maxSpeed, m_velocity.y * m_maxSpeed);
	m_desiredVelocity = m_target - m_position;
	m_desiredVelocity = m_desiredVelocity.normalise() * m_maxSpeed;
	m_steering = m_desiredVelocity - m_velocity;

	m_steering = truncate(m_steering, 10.0f);
	m_steering = m_steering / m_mass;
	m_velocity = truncate(m_velocity + m_steering, m_maxSpeed);
}
/**
*function used to calculate the flee behaviour
*/
void AI::flee()
{
	m_velocity = m_velocity.normalise();
	m_velocity = sf::Vector2f(m_velocity.x * m_maxSpeed, m_velocity.y * m_maxSpeed);
	m_desiredVelocity = m_position - m_target;
	m_desiredVelocity = m_desiredVelocity.normalise() * m_maxSpeed;
	m_steering = m_desiredVelocity - m_velocity;

	m_steering = truncate(m_steering, 10.0f);
	m_steering = m_steering / m_mass;
	m_velocity = truncate(m_velocity + m_steering, m_maxSpeed);
}
/**
*function used to calculate the wander behaviour
*/
void AI::wander()
{
	// Calculate the circle center
	m_circleCenter = m_velocity;

	m_circleCenter = m_circleCenter.normalise();
	m_circleCenter *= CIRCLE_DISTANCE;

	// Randomly change the vector direction
	// by making it change its current angle
	m_displacement = Vector2f(0, -1);
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

	// Calculate the wander force#
	m_steering = m_wanderForce;
	m_steering = truncate(m_steering, 10.0f);
	m_velocity = truncate(m_velocity + m_steering, m_maxSpeed);
}
