#include "Sweeper.h"

Sweeper::Sweeper(Vector2f& playerPos, Grid* grid, Vector2f pos, std::vector<Worker>* workers) :
	m_gridPtr(grid),
	m_playerPosPtr(&playerPos),
	m_playerPosPair(playerPos, false),
	m_currentWorker(0),
	m_position(pos.x, pos.y),
	m_velocity(0, 0),
	m_target(nullptr),
	m_state(SweeperState::Seek),
	m_moveSpeed(75.f),
	m_maxSpeed(100.f),
	m_turnSpeed(1100.f),
	m_angle(),
	m_gridRect(),
	m_alive(true),
	m_rangeCollider(m_position.x, m_position.y, 40),
	m_body(Type::Static, Shape::Circle, this)
{
	std::random_shuffle(workers->begin(), workers->end());
	m_seekWorkers = workers;
	m_body.setFriction(0.985f);
	m_body.setRestitution(.1f);
	m_body.mask = 3;
	m_body.bitmasks.push_back(3);
	m_body.setCircleParameters(m_position, 40, 1.25f, false);
	m_body.tag = "Sweeper";
}

Sweeper::~Sweeper()
{
}

void Sweeper::update(float dt)
{
	if (m_alive)
	{
		m_playerPosPair.pos = *m_playerPosPtr; 

		if (m_state == SweeperState::Seek)
		{
			//If we are not in range of the player
			if (m_position.distance(*m_playerPosPtr) < 300)
			{
				m_seekPath.clear(); //Clear the seek path
				m_currentWorker = 0;
				m_state = SweeperState::Flee;
			}
			else
			{
				//If our path is empty do a BFS to the player from our grid position
				if (m_seekPath.size() == 0 && m_seekWorkers->size() !=0)
				{
					auto workerGridPos = sf::Vector2i(int(floor(m_seekWorkers->at(m_currentWorker).getPos().x / 160.0f)),
													  int(floor((m_seekWorkers->at(m_currentWorker).getPos().y / 160.0f))));
					auto myGridPos = sf::Vector2i(int(floor(m_position.x / 160.0f)), int(floor(m_position.y / 160.0f)));

					auto pGridS = m_gridPtr->convertToString(workerGridPos); //Get string verson of grid position
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
						if (m_currentWorker < m_seekWorkers->size())
						{
							m_currentWorker++;
						}
						if (m_currentWorker >= m_seekWorkers->size())
						{
							m_currentWorker = 0;
						}
					}
				}

			}
		}
		else
		{
			fleePlayer(m_playerPosPair, dt);
		}

		//If our speed has gone past our max speed, clamp our speed to our max speed
		if (m_body.velocity.magnitude() > m_maxSpeed)
			m_body.velocity = m_body.velocity.normalise() * m_maxSpeed;

		//Update the positions of our colliders and sprites
		m_position = m_body.position; //Get position of the body
		m_sprite.setPosition(m_position.x, m_position.y);
		m_sprite.setRotation(m_angle+90);
		//Update grid rectangle so we can check what grid we are in
		m_gridRect.left = m_position.x - m_gridRect.width / 2;
		m_gridRect.top = m_position.y - m_gridRect.height / 2;
		m_rangeCollider.setPosition(m_position);
	}


}
void Sweeper::render(sf::RenderWindow& window)
{
	if (m_alive)
	{
		window.draw(m_sprite);
	}
}

bool Sweeper::seek(boolVecPair& p, double dt)
{
	//If target is outside the follow distance, then seek
	if (p.pos.distance(p.pos, m_position) > 10.0f)
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

		Vector2f tempPos = m_seekWorkers->at(m_currentWorker).getPos();
		if (m_position.distance(m_position, tempPos) < 50.0f && m_seekWorkers->at(m_currentWorker).captured() == false)
		{
			m_capturedWorkers.push_back(&m_seekWorkers->at(m_currentWorker));
			m_seekWorkers->at(m_currentWorker).captureWorker(false, m_position);
			p.visited = true;
			m_seekPath.clear();
		}
		return false; //havnt reached target yet
	}
	else
	{
		p.visited = true;
		//m_body.velocity.zeroVector(); //Reset velocity on the worker
		return true; //Reached Target
	}
}
bool Sweeper::fleePlayer(boolVecPair& p, double dt)
{
	//If target is outside the follow distance, then seek
	if (p.pos.distance(p.pos, m_position) < 600.0f)
	{
		m_velocity = m_body.velocity.normalise();
		m_velocity *= m_moveSpeed * dt;
		Vector2f desiredVelocity = (m_position - p.pos).normalise() * m_turnSpeed * dt;
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
		m_state = SweeperState::Seek;
		p.visited = true;
		//m_body.velocity.zeroVector(); //Reset velocity on the worker
		return true; //Reached Target
	}
}


void Sweeper::setTexture(ResourceManager & resources)
{
	m_sprite.setTexture(resources.getTexture("Sweeper"));
	m_sprite.setOrigin(m_sprite.getLocalBounds().width / 2, m_sprite.getLocalBounds().height / 2);

	m_body.objectData = this; //Set object data here
	physics::world->addPhysicsBody(m_body); //Add to physics here, as we cannot modify the physics during a physics update
}

void Sweeper::die()
{
	//for (int i = 0; i < m_capturedWorkers->size(); i++)
	//{
	//	m_capturedWorkers->at(i).freeWorker();
	//}
	m_body.position.zeroVector();
	m_body.velocity.zeroVector();
	m_body.type = Type::Static; //Swap to static, as we dont need collision detection anymore when dead
}
