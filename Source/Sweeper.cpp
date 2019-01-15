#include "Sweeper.h"



Sweeper::Sweeper(Player* p, float maxSpeed, Vector2f pos)
{
	m_player = p;
	m_maxSpeed = maxSpeed;
	m_position = pos;
	m_velocity = Vector2f(0, 0);
	m_target = Vector2f(0, 0);
	m_desiredVelocity = Vector2f(0, 0);
	m_steering = Vector2f(0, 0);
	m_orientation = 0;
	m_mass = 1;
	m_circleCenter = Vector2f(0, 0);
	m_wanderForce = Vector2f(0, 0);
	m_displacement = Vector2f(0, 0);
}

Sweeper::~Sweeper()
{
}

void Sweeper::update(float dt)
{
	if (m_seeWorker == false)
	{
		wander();
	}
	else
	{
		setTarget(m_player->m_position);
		seek();
	}


	m_position = m_position + (m_velocity * dt);
	m_sprite.setPosition(sf::Vector2f(m_position.x, m_position.y));
	m_orientation = getNewOrientationByVelocity(m_orientation, m_velocity);
	m_sprite.setRotation(m_orientation);

	//If our speed has gone past our max speed, clamp our speed to our max speed
	/*if (m_physicsBody.velocity.magnitude() > m_maxSpeed)
		m_physicsBody.velocity = m_physicsBody.velocity.normalise() * m_maxSpeed;*/

		//m_rangeCollider.left = m_position.x - m_rangeCollider.width / 2;
	//m_rangeCollider.top = m_position.y - m_rangeCollider.height / 2;

	//std::cout << m_position.x << m_position.y << std::endl;
}

void Sweeper::render(sf::RenderWindow & window)
{
	window.draw(m_sprite);
}
