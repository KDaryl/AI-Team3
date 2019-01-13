#include "Flee.h"



Flee::Flee(Vector2f position, float maxSpeed)
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


Flee::~Flee()
{
}

void Flee::update(Vector2f t)
{
	//m_target is the player position 
	m_target = t;
	m_velocity = vectorNormalise(m_velocity);
	m_velocity = sf::Vector2f(m_velocity.x * m_maxSpeed, m_velocity.y * m_maxSpeed);
	m_desiredVelocity = vectorNormalise(m_position - m_target) * m_maxSpeed;
	m_steering = m_desiredVelocity - m_velocity;

	m_steering = truncate(m_steering, 10.0f);
	m_steering = m_steering / m_mass;
	m_velocity = truncate(m_velocity + m_steering, m_maxSpeed);
	m_position = m_position + m_velocity;


	//If our speed has gone past our max speed, clamp our speed to our max speed
	/*if (m_physicsBody.velocity.magnitude() > m_maxSpeed)
		m_physicsBody.velocity = m_physicsBody.velocity.normalise() * m_maxSpeed;*/

	m_sprite.setPosition(sf::Vector2f(m_position.x, m_position.y));
	m_orientation = getNewOrientationByVelocity(m_orientation, m_velocity);
	m_sprite.setRotation(m_orientation);

	//m_rangeCollider.left = m_position.x - m_rangeCollider.width / 2;
	//m_rangeCollider.top = m_position.y - m_rangeCollider.height / 2;

	std::cout << m_position.x << m_position.y << std::endl;
}

void Flee::render(sf::RenderWindow & window)
{
	window.draw(m_sprite);
}
