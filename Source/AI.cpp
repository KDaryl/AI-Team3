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

Vector2f AI::truncate(Vector2f v, float max)
{
	float i = 0;
	i = max / vectorLength(v);
	i = i < 1.0 ? i : 1.0;
	v *= i;
	return v;
}
float AI::getNewOrientationByPosition(float currentOrientation, Vector2f currentVelocity)
{
	// Note atan2 returns an angle in radians which you 
	// may want to convert to degrees.
	if (vectorLength(currentVelocity) > 0)
	{
		const auto radAngle = atan2(-m_sprite.getPosition().y, m_sprite.getPosition().x);
		return (radAngle)* (180 / M_PI);
	}
	else
	{
		return currentOrientation;
	}
}
float AI::getNewOrientationByVelocity(float currentOrientation, Vector2f currentVelocity)
{
	// Note atan2 returns an angle in radians which you 
	// may want to convert to degrees.
	if (vectorLength(currentVelocity) > 0)
	{
		return (atan2(-currentVelocity.x, currentVelocity.y))* (180.0f / M_PI) + 180;
	}
	else
	{
		return currentOrientation;
	}
}
void AI::setAngle(Vector2f & v, float f)
{
	float len = vectorLength(v);
	v.x = cos(f * (M_PI / 180.0f)) * len;
	v.y = sin(f* (M_PI / 180.0f)) * len;
}

sf::Sprite & AI::getSprite()
{
	return m_sprite;
}

Vector2f AI::getVelocity()
{
	return m_velocity;
}

void AI::setTexture(ResourceManager & resources, std::string name, Vector2f center)
{
	m_sprite.setTexture(resources.getTexture(name));
	m_sprite.setPosition(sf::Vector2f(m_position.x, m_position.y));
	//Set center of sprite
	m_sprite.setOrigin(sf::Vector2f(center.x,center.y));
}

float AI::vectorLength(Vector2f v)
{
	float magnitudeV = sqrt((v.x * v.x) + (v.y * v.y));
	return magnitudeV;
}

Vector2f AI::vectorNormalise(Vector2f v)
{
	float mag = vectorLength(v);
	if (mag != 0)
	{
		return Vector2f(v.x / mag, v.y / mag);
	}
	else
	{
		return Vector2f(v.x, v.y);
	}
}
