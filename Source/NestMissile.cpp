#include "NestMissile.h"

NestMissile::NestMissile(Vector2f& playerPos, Grid* grid) :
	alive(false),
	collided(false),
	dead(false),
	m_gridPtr(grid),
	m_playerPosPtr(&playerPos),
	m_playerPosPair(playerPos, false),
	m_position(0,0),
	m_moveSpeed(150.f),
	m_angle(0),
	timeAlive(0),
	m_maxSpeed(240.f),
	m_turnSpeed(750.f),
	m_body(Type::Static, Shape::Circle, this)
{
	m_body.setCircleParameters(m_position, 5, 1, false);
	m_body.tag = "Nest Missile";
	m_body.mask = 3;
	m_body.mass = 1;
	m_body.bitmasks.push_back(3); //Avoid collisions with predator ships and enemy bullets
	m_body.isSensor = true; //Is a sensor, means it will avouid collision resolution (wont bounce)
}

/**
* Description: Updates the nest missile and pathfinds to the player if he is out of range, else he seeks to the player
*/
void NestMissile::update(double dt)
{
	if (timeAlive >= ttl)
		hasCollided();

	//Update animator
	m_animator.update(sf::seconds(dt));
	m_animator.animate(m_sprite);

	//If alive, play our moving animation
	if (alive)
	{
		timeAlive += dt; //Add to our time alive

		//Play our moving animation
		if (m_animator.isPlayingAnimation() && m_animator.getPlayingAnimation() != "Move" || !m_animator.isPlayingAnimation())
		{
			m_animator.stopAnimation(); //Stop playing current animation
			m_animator.playAnimation("Move", true); //Play our moving animation
		}

		m_playerPosPair.pos = *m_playerPosPtr;


		//If we are not in range of the player
		if (m_position.distance(*m_playerPosPtr) > 640)
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
		}
		else
		{
			m_target = &m_playerPosPair;
		}

		//Seek to the set target
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

		//If our speed has gone past our max speed, clamp our speed to our max speed
		if (m_body.velocity.magnitude() > m_maxSpeed)
			m_body.velocity = m_body.velocity.normalise() * m_maxSpeed;

		//Update the positions of our colliders and sprites
		m_position = m_body.position; //Get position of the body
		m_sprite.setPosition(m_position.x, m_position.y);
		m_sprite.setRotation(m_angle + 90);
	}
	else if (collided) //If we have collided with something, play our dead animation
	{
		//Play our idle animation if we are not moving
		if (!dead && (m_animator.isPlayingAnimation() && m_animator.getPlayingAnimation() != "Dead" || !m_animator.isPlayingAnimation()))
		{
			dead = true;
			m_animator.stopAnimation(); //Stop playing current animation
			m_animator.playAnimation("Dead"); //Play our dead animation
		}
		else if (!m_animator.isPlayingAnimation())
		{
			collided = false;
		}
	}
}

void NestMissile::draw(sf::RenderWindow & window)
{
	//Only draw if the animations are playing
	if (alive || collided)
	{
		window.draw(m_sprite);
	}
}

/**
* Description: spawns the missile at the position and angle passsed
*/
void NestMissile::spawn(Vector2f position, float angle)
{
	alive = true;
	collided = false;
	dead = false;
	m_position = position;
	m_angle = angle;
	auto rad = thor::toRadian(m_angle);
	m_velocity = Vector2f(cos(rad), sin(rad));

	//Reset velocity and position of the bullets body
	m_body.position = m_position;
	m_body.velocity.zeroVector();

	m_sprite.setPosition(m_position.x, m_position.y);
	m_sprite.setRotation(m_angle + 90);

	m_body.type = Type::Dynamic; //make the body dynamic as it can move now

	//Play our moving animation
	if (m_animator.isPlayingAnimation() && m_animator.getPlayingAnimation() != "Move" || !m_animator.isPlayingAnimation())
	{
		m_animator.stopAnimation(); //Stop playing current animation
		m_animator.playAnimation("Move", true); //Play our moving animation
	}
}

void NestMissile::setTexture(ResourceManager & resources)
{
	m_sprite.setTexture(resources.getTexture("Nest Missile"));
	m_sprite.setOrigin(37.5f, 37.5f); //Center sprite

	thor::FrameAnimation move, dead; //Our animations

									 //Create our move animation
	move.addFrame(0.1f, sf::IntRect(0, 0, 75, 75));

	//Create our move animation
	for (int i = 0; i < 20; i++)
	{
		dead.addFrame(0.1f, sf::IntRect((i * 75), 0, 75, 75));
	}

	//Add our animations to our animator
	m_animator.addAnimation("Move", move, sf::seconds(1));
	m_animator.addAnimation("Dead", dead, sf::seconds(0.2f));

	m_body.objectData = this;
	//Add our physics body
	physics::world->addPhysicsBody(m_body);
}


/**
* Description: Sets the missile as collided
*/
void NestMissile::hasCollided()
{
	alive = false;
	collided = true;
	timeAlive = 0;
	m_body.position.zeroVector();
	m_body.velocity.zeroVector();
	m_body.type = Type::Static; //Change to static as we dont want physics to take over
}

/**
* Description: seeks to a position
*/
bool NestMissile::seek(boolVecPair & p, double dt)
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
		return true; //Reached Target
	}
}
