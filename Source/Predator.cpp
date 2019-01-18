#include "Predator.h"

Predator::Predator(Vector2f& playerPos, Grid* grid, int idIn) :
	m_gridPtr(grid),
	m_playerPosPtr(&playerPos),
	m_flockVector(0,0),
	m_id(idIn),
	m_playerPosPair(playerPos, false),
	m_position(0, 0),
	m_velocity(0,0),
	m_target(nullptr),
	m_state(PredatorState::Seek),
	m_moveSpeed(150.f),
	m_maxSpeed(240.f),
	m_turnSpeed(1100.f),
	m_angle(0),
	m_gridRect(),
	m_alive(false),
	m_fireRate(1),
	m_timeToFire(m_fireRate),
	m_rangeCollider(m_position.x, m_position.y, 40),
	m_body(Type::Static, Shape::Circle, this)
{
	m_body.setFriction(0.985f);
	m_body.setRestitution(.1f);
	m_body.mask = 3;
	m_body.bitmasks.push_back(3);
	m_body.setCircleParameters(m_position, 40, 1.25f, false);
	m_body.tag = "Predator";

	for (int i = 0; i < 2; i++)
	{
		m_bullets.push_back(EnemyBullet());
	}
}

Predator::~Predator()
{
}

void Predator::update(double dt)
{
	if (m_alive)
	{
		m_timeToFire += dt; //Add to our time to fire

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

	//Update any live bullets
	for (auto& bullet : m_bullets)
	{
		bullet.update(dt);
	}
}

void Predator::draw(sf::RenderWindow & window)
{
	//Draw any live bullets
	for (auto& bullet : m_bullets)
	{
		bullet.draw(window);
	}

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
		m_velocity = m_body.velocity.normalise();
		m_velocity *= m_moveSpeed * dt;
		Vector2f desiredVelocity = (p.pos - m_position).normalise() * m_turnSpeed * dt;
		Vector2f steering = desiredVelocity - m_velocity;

		steering = AI::truncate(steering, m_turnSpeed);
		steering /= m_body.mass;
		steering = AI::truncate(m_velocity + steering, m_maxSpeed);

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

	if (m_timeToFire >= m_fireRate)
	{


		//Spawn a bullet
		for (auto& bullet : m_bullets)
		{
			//If the bullet is not alive and currently colliding
			if (bullet.alive == false && bullet.collided == false)
			{
				m_timeToFire = 0; //Reset time to fire
				bullet.spawn(m_position, m_angle);
				break;
			}
		}
	}
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

	//Set bullet textures
	for (auto& bullet : m_bullets)
	{
		bullet.setTexture(resources);
	}

	m_body.objectData = this; //Set object data here
	physics::world->addPhysicsBody(m_body); //Add to physics here, as we cannot modify the physics during a physics update
}

void Predator::die()
{
	m_body.position.zeroVector();
	m_body.velocity.zeroVector();
	m_body.type = Type::Static; //Swap to static, as we dont need collision detection anymore when dead
}


void Predator::flock(std::vector<Predator>* p, int currentP)
{
	Vector2f sep = separation(p, currentP);
	Vector2f ali = alignment(p, currentP);
	Vector2f coh = cohesion(p, currentP);

	p->at(currentP).m_velocity.x += (ali.x * aliWeight) + (coh.x * cohWeight) + (sep.x * sepWeight);
	p->at(currentP).m_velocity.y += (ali.y * aliWeight) + (coh.y * cohWeight) + (sep.y * sepWeight);

	p->at(currentP).m_velocity.normalise();
}

Vector2f Predator::separation(std::vector<Predator>* p, int currentP)
{
	m_neighbours = 0;
	m_flockVector = Vector2f(0,0);

	//for every predator 
	for (int i = 0; i < p->size(); i++)
	{
	
		//if the predator we are comparing to is not the current one 
		if (p->at(i).m_id == p->at(currentP).m_id)
		{
			//if the distance between the predators is less than some value
			if (p->at(i).m_position.distance(p->at(i).m_position, p->at(currentP).m_position) < distanceBetweenMax)
			{
				//add the velocity of the compared predator to a vector and increase our neighbors
				m_flockVector.x += p->at(i).m_position.x - p->at(currentP).m_position.x;
				m_flockVector.y += p->at(i).m_position.y - p->at(currentP).m_position.y;
				m_neighbours++;
			}

		}
	}
	//if there are no neighbors no need to do anything
	if (m_neighbours == 0)
	{
		return Vector2f(0, 0);
	}

	m_flockVector.x /= m_neighbours;
	m_flockVector.y /= m_neighbours;
	//steer away from neighbors
	m_flockVector.x *= -1;
	m_flockVector.y *= -1;
	m_flockVector.normalise();
	return m_flockVector;
}

Vector2f Predator::alignment(std::vector<Predator>* p, int currentP)
{
	m_neighbours = 0;
	m_flockVector = Vector2f(0, 0);

//	for every predator 
	for(int i = 0; i < p->size(); i++)
	{
		//if the predator we are comparing to is not the current one 
		if (p->at(i).m_id == p->at(currentP).m_id)
		{
		//	if the distance between the predators is less than some value
			if (p->at(i).m_position.distance(p->at(i).m_position, p->at(currentP).m_position) < distanceBetweenMax)
			{
			//	add the velocity of the compared predator to a vector and increase our neighbors
				m_flockVector.x += p->at(i).m_velocity.x;
				m_flockVector.y += p->at(i).m_velocity.y;
				m_neighbours++;
			}

		}
	}
	//if there are no neighbors no need to do anything
	if (m_neighbours == 0)
	{
		return Vector2f(0, 0);
	}
	
	m_flockVector.x /= m_neighbours;
	m_flockVector.y /= m_neighbours;
	m_flockVector.normalise();
	return m_flockVector;
}

Vector2f Predator::cohesion(std::vector<Predator>* p, int currentP)
{
	m_neighbours = 0;
	m_flockVector = Vector2f(0, 0);

	//for every predator 
	for (int i = 0; i < p->size(); i++)
	{
	//	if the predator we are comparing to is not the current one 
		if (p->at(i).m_id == p->at(currentP).m_id)
		{
		//	if the distance between the predators is less than some value
			if (p->at(i).m_position.distance(p->at(i).m_position, p->at(currentP).m_position) < distanceBetweenMax)
			{
			//	add the velocity of the compared predator to a vector and increase our neighbors
				m_flockVector.x += p->at(i).m_position.x;
				m_flockVector.y += p->at(i).m_position.y;
				m_neighbours++;
			}

		}
	}
	//if there are no neighbors no need to do anything
	if (m_neighbours == 0)
	{
		return Vector2f(0, 0);
	}

	m_flockVector.x /= m_neighbours;
	m_flockVector.y /= m_neighbours;
	m_flockVector = Vector2f(m_flockVector.x - p->at(currentP).m_position.x , m_flockVector.y - p->at(currentP).m_position.y);
	m_flockVector.normalise();
	return m_flockVector;
}
