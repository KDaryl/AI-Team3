#include "Nest.h"

Nest::Nest(Player& player, Grid& grid, Vector2f pos, std::vector<Predator>& preds) :
	m_position(pos),
	m_currentBuildTime(0),
	m_alive(true),
	m_isBuilding(false),
	m_timeTillFire(m_fireRate),
	m_predators(&preds), //Set predator pointer
	m_missile(player.m_position, &grid), //Setup our missile
	m_playerPtr(&player),
	m_body(Type::Dynamic, Shape::Circle, this)
{

	m_body.setCircleParameters(m_position, 60, 0, false);
	m_body.bitmasks.push_back(3);
	m_body.tag = "Nest";
	m_body.type = Static;
}

Nest::~Nest()
{
}

void Nest::update(double dt, std::vector<Nest>& otherNests)
{
	//Draw our missile
	m_missile.update(dt);

	//If we are alive
	if (m_alive)
	{
		//Loop through the predators
		for (auto& pred : *m_predators)
		{
			if (pred.isSpawned() == false)
			{
				//Loop through our nests
				for (auto& otherN : otherNests)
				{
					//If we are not the same nest and the nest is not building anything then start building a predator
					if (otherN.m_position != m_position && otherN.isBuilding() == false)
					{
						m_isBuilding = true;
						m_predToSpawn = &pred;
						break;
					}
				}
			}
		}

		//If we are building a predator
		if (m_isBuilding)
		{
			m_currentBuildTime += dt;

			//If we have reache dthe build time
			if (m_currentBuildTime >= m_buildTime)
			{
				m_predToSpawn->spawn(m_position);
				m_currentBuildTime = 0;
				m_isBuilding = false;
			}
		}

		m_timeTillFire += dt;

		if (m_timeTillFire >= m_fireRate)
		{

			if (m_position.distance(m_playerPtr->m_position) < 640)
			{
				if (m_missile.alive == false)
				{
					m_missile.spawn(m_position, 0);
					m_timeTillFire = 0;
				}
			}
		}
	}
}

void Nest::draw(sf::RenderWindow & window)
{
	if (m_alive)
	{
		window.draw(m_sprite);
	}

	m_missile.draw(window);
}

/**
* Description: Kills the nest
*/
void Nest::die()
{
	m_alive = false;
	physics::world->deletePhysicsBody(m_body);
}

void Nest::setTexture(ResourceManager & resources)
{
	m_sprite.setTexture(resources.getTexture("Alien Nest"));
	m_sprite.setOrigin(m_sprite.getLocalBounds().width / 2, m_sprite.getLocalBounds().height / 2);
	m_sprite.setPosition(m_position.x, m_position.y);

	//Set our collider
	m_collider = sf::FloatRect(m_position.x - m_sprite.getGlobalBounds().width / 2, m_position.y - m_sprite.getGlobalBounds().height / 2, m_sprite.getGlobalBounds().width, m_sprite.getGlobalBounds().height);
	
	//set missile texture
	m_missile.setTexture(resources);

	m_body.objectData = this;
	physics::world->addPhysicsBody(m_body);
}

/**
* Description: Here we add or decrement the health of the nest
*/
void Nest::addDecHealth(int val)
{
	//Add value to our health
	m_health += val;

	if (m_health <= 0)
	{
		m_health = 0;
		die();
	}
	else if (m_health > 100)
		m_health = 100;
}
