#include "Predator.h"

Predator::Predator(Vector2f& playerPos, Grid* grid) :
	m_gridPtr(grid),
	m_playerPosPtr(&playerPos),
	m_playerPosPair(playerPos, false),
	m_position(0, 0),
	m_target(nullptr),
	m_state(PredatorState::Seek),
	m_moveSpeed(150.f),
	m_maxSpeed(240.f),
	m_turnSpeed(1100.f),
	m_angle(0),
	m_gridRect(),
	m_alive(false),
	m_rangeCollider(m_position.x, m_position.y, 40),
	m_body(Type::Static, Shape::Circle, this)
{
	m_body.setFriction(0.985f);
	m_body.setRestitution(.1f);
	m_body.setCircleParameters(m_position, 40, 1.25f, false);
	m_body.tag = "Predator";
}

Predator::~Predator()
{
}

void Predator::update(double dt)
{
	if (m_alive)
	{
		m_playerPosPair.pos = *m_playerPosPtr; //Update our player pos pair

		//If seeking, check if we are in range of the player, if so, attack, else path find to player
		if (m_state == PredatorState::Seek)
		{
			//If we are not in range of the player
			if (m_position.distance(*m_playerPosPtr) > 320)
			{
				//If our path is empty do a BFS to the player from our grid position
				if (m_seekPath.size() == 0)
				{
					auto playerGridPos = sf::Vector2i(int(floor(m_playerPosPtr->x / 160.0f)), int(floor(m_playerPosPtr->y / 160.0f)));
					auto myGridPos = sf::Vector2i(int(floor(m_position.x / 160.0f)), int(floor(m_position.y / 160.0f)));

					auto pGridS = m_gridPtr->convertToString(playerGridPos); //Get string verson of grid position
					auto myGridS = m_gridPtr->convertToString(myGridPos); //Get my grid position in a string

					for (auto& pos : m_gridPtr->BFS(m_gridPtr->m_cells[myGridS], m_gridPtr->m_cells[pGridS], m_gridPtr->m_cellsPtrs))
					{
						m_seekPath.push_back(boolVecPair(pos, false));
					}

					m_target = &m_seekPath.at(0);
				}

				//If seek returns true 
				if (seek(*m_target, dt))
				{
					//Remove any positions we have already visited
					m_seekPath.erase(std::remove_if(m_seekPath.begin(), m_seekPath.end(), removeVisited()),
						m_seekPath.end());

					//If we have no more positions to seek to, then look for the player
					if (m_seekPath.size() == 0)
					{
						m_target = &m_playerPosPair;
					}
				}
		
			}
			else
			{
				m_seekPath.clear(); //Clear the seek path

				//Since player is in view, look at them
				lookAtPlayer(dt);
			}
		}

		//If our speed has gone past our max speed, clamp our speed to our max speed
		if (m_body.velocity.magnitude() > m_maxSpeed)
			m_body.velocity = m_body.velocity.normalise() * m_maxSpeed;

		//Update the positions of our colliders and sprites
		m_position = m_body.position; //Get position of the body
		m_sprite.setPosition(m_position.x, m_position.y);
		m_sprite.setRotation(m_angle);
		//Update grid rectangle so we can check what grid we are in
		m_gridRect.left = m_position.x - m_gridRect.width / 2;
		m_gridRect.top = m_position.y - m_gridRect.height / 2;
		m_rangeCollider.setPosition(m_position);
	}
}

void Predator::draw(sf::RenderWindow & window)
{
	if (m_alive)
	{
		window.draw(m_sprite);
	}

}

bool Predator::seek(boolVecPair& p, double dt)
{
	//If target is outside the follow distance, then seek
	if (p.pos.distance(p.pos, m_position) > 40.0f)
	{
		Vector2f velocity = m_body.velocity.normalise();
		velocity *= m_moveSpeed * dt;
		Vector2f desiredVelocity = (p.pos - m_position).normalise() * m_turnSpeed * dt;
		Vector2f steering = desiredVelocity - velocity;

		steering = AI::truncate(steering, m_turnSpeed);
		steering /= m_body.mass;
		steering = AI::truncate(velocity + steering, m_maxSpeed);

		//Dont need to multiplty by dt as we already have in the previous calculation
		m_body.addForce(steering); 

		m_angle = AI::getOrientation(m_angle, m_body.velocity);

		return false; //havnt reached target yet
	}
	else
	{
		p.visited = true;
		//m_body.velocity.zeroVector(); //Reset velocity on the worker
		return true; //Reached Target
	}
}

void Predator::lookAtPlayer(double dt)
{
	m_angle = (*m_playerPosPtr - m_position).angle();
}

void Predator::spawn(Vector2f spawnPos)
{
	m_alive = true;
	m_body.type = Type::Dynamic; //Sawp to dynamic
	m_body.position = spawnPos;
	m_position = spawnPos;
	//Update grid rectangle so we can check what grid we are in
	m_gridRect.left = m_position.x - m_gridRect.width / 2;
	m_gridRect.top = m_position.y - m_gridRect.height / 2;
}

void Predator::setTexture(ResourceManager & resources)
{
	m_sprite.setTexture(resources.getTexture("Predator"));
	m_sprite.setOrigin(m_sprite.getLocalBounds().width / 2, m_sprite.getLocalBounds().height / 2);

	m_body.objectData = this; //Set object data here
	physics::world->addPhysicsBody(m_body); //Add to physics here, as we cannot modify the physics during a physics update
}

void Predator::die()
{
	m_body.position.zeroVector();
	m_body.velocity.zeroVector();
	m_body.type = Type::Static; //Swap to static, as we dont need collision detection anymore when dead
}
