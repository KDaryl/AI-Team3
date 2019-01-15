#include "Worker.h"

Worker::Worker(Vector2f startPos, Grid* grid) :
	m_gridPtr(grid),
	m_position(startPos),
	m_target(startPos),
	m_startPos(startPos),
	m_state(WorkerState::Wander),
	m_moving(false),
	m_moveSpeed(125), //Can move 250 Pixels per second
	m_maxSpeed(75), //Max speed you can move per second
	m_turnSpeed(180), //Can turn 180 Degrees a second
	m_angle(-90),
	m_wanderChange(4), //Wander to new position every 2 seconds
	m_currentWander(m_wanderChange), //Current time spent moving to wander position
	m_gridRect(startPos.x - 20, startPos.y - 20, 40, 40),
	m_body(Type::Dynamic, Shape::Circle, this),
	m_rangeCollider(startPos.x, startPos.y, 20),
	m_captured(false)
{

	m_body.setCircleParameters(m_startPos, 10, .5, false);
	m_body.setFriction(1);
	m_body.setRestitution(1);

	//Loop through split grid and see where abouts we are
	for (auto& cell : m_gridPtr->m_splitGridcells)
	{
		//Which of the 16 overall grids are we in
		if (cell.second.rect.intersects(m_gridRect))
		{
			std::string gridS = std::to_string(cell.second.gridPosition.x) + "," + std::to_string(cell.second.gridPosition.y);

			for (auto& smallCell : m_gridPtr->m_splitCells[gridS])
			{
				if (smallCell->rect.intersects(m_gridRect))
				{
					m_gridPosition = smallCell->gridPosition; //Set our Grid Position
				}
			}
		}
	}
}

void Worker::update(double dt)
{
	m_moving = false; //Set moving to false at the start of every update
	m_animator.update(sf::seconds(dt));	//Update Animator
	m_animator.animate(m_sprite); //Animate our sprite

	if (m_state == WorkerState::Wander) //If wander, wander the general aread
	{
		m_currentWander += dt; //Add delta time to our wander time

		//If we have been wandering for the duration of our wander, then change to wander to a new position
		if (m_currentWander >= m_wanderChange)
		{
			bool changeX = randInt(0, 1); //Get random bool for wheter to change the X or Y

			//Generate random numbers and get a new grid position
			auto newPos = (changeX ? sf::Vector2i(randInt(-1, 1), 0) : sf::Vector2i(0, randInt(-1, 1))) + m_gridPosition; //Get new position and add current grid position to it

			auto gridS = m_gridPtr->convertToString(newPos);

			//If new position is actually in the grid and its not too far away from our starting point and it isnt a wall
			if (newPos.x >= 0 && newPos.x < 72 && newPos.y >= 0 && newPos.y < 41 &&
				m_gridPtr->m_cells[gridS].isWall == false &&
				m_startPos.distance(m_gridPtr->m_cells[gridS].position) <= 200)
			{
				m_seekPath = m_gridPtr->BFS(m_gridPtr->m_cells[m_gridPtr->convertToString(m_gridPosition)], m_gridPtr->m_cells[gridS], m_gridPtr->m_cellsPtrs);

				m_target = *m_seekPath.begin(); //Set target to first element of the seek path

				m_currentWander = 0;	
			}
		}

		//If seeking returns true, it means we have reached our destination
		seek(m_target, dt); //Seek to target	
	}

	else if (m_state == WorkerState::Captured) //If captured, seek to capture's position
	{
		if (nullptr != capturePos) //As long as the pointer is not null, seek to it
		{
			seek(*capturePos, dt);
		}
	}

	//If the worker is moving, play the move animation
	if (m_moving)
	{
		//Play our moving animation
		if (m_animator.isPlayingAnimation() && m_animator.getPlayingAnimation() != "Moving" || !m_animator.isPlayingAnimation())
		{
			m_animator.stopAnimation(); //Stop playing current animation
			m_animator.playAnimation("Moving", true); //Play our moving animation
		}
	}
	//Else play the idle animation
	else if(m_animator.isPlayingAnimation() && m_animator.getPlayingAnimation() != "Idle" || !m_animator.isPlayingAnimation())
	{
		m_animator.stopAnimation(); //Stop playing current animation
		m_animator.playAnimation("Idle"); //Play our idle animationaw
	}

	m_position = m_body.position; //Get Position of the body

	//If our speed has gone past our max speed, clamp our speed to our max speed
	if (m_body.velocity.magnitude() > m_maxSpeed)
		m_body.velocity = m_body.velocity.normalise() * m_maxSpeed;

	//Update sprite position and angle
	m_sprite.setPosition(m_position.x, m_position.y);
	m_sprite.setRotation(m_angle + 90); //Add offset of 90 due to the sprite being faced the wrong way
	//Update grid rectangle so we can check what grid we are in
	m_gridRect.left = m_position.x - m_gridRect.width / 2;
	m_gridRect.top = m_position.y - m_gridRect.height / 2;
	m_rangeCollider.setPosition(m_position); //Set range Collider
}

void Worker::draw(sf::RenderWindow & window)
{
	window.draw(m_sprite);
}

bool Worker::seek(Vector2f pos, double dt)
{
	//If target is outside the follow distance, then seek
	if (pos.distance(pos, m_position) > m_followDistance)
	{
		m_moving = true;

		Vector2f velocity = m_body.velocity.normalise();
		velocity *= m_moveSpeed;
		Vector2f desiredVelocity = (pos - m_position).normalise() * m_moveSpeed;
		Vector2f steering = desiredVelocity - velocity;

		steering = AI::truncate(steering, m_turnSpeed);
		steering /= m_body.mass;
		steering = AI::truncate(velocity + steering, m_maxSpeed);
		
		m_body.addForce(steering * dt);

		m_angle = AI::getOrientation(m_angle, m_body.velocity);

		return false; //havnt reached target yet
	}
	else
	{
		m_body.velocity.zeroVector(); //Reset velocity on the worker
		return true; //Reached Target
	}
}

void Worker::captureWorker(bool isPlayer, Vector2f & position)
{
	if (isPlayer)
	{
		m_playerCaptured = true;
	}
	else
	{
		m_state = WorkerState::Captured;
		m_captured = true;
		m_followDistance = 50;
		capturePos = &position; //Point to that position, this way we can follow it easily
	}
}

void Worker::freeWorker()
{
	m_startPos = m_position; //Set start to where the position of the worke ris currently
	capturePos = nullptr;
	m_captured = false;
	m_playerCaptured = false;
	m_followDistance = 12.5f;
	m_currentWander = m_wanderChange; //Set our current wander to wander change, this will figure out a place to go
	m_state = WorkerState::Wander;
}

void Worker::setTexture(ResourceManager & resources)
{
	auto width = (238 / 6); //Width of 1 frame
	m_sprite.setTexture(resources.getTexture("Worker"));
	m_sprite.setOrigin(width / 2, 21); //Setting the origin
	m_sprite.setPosition(m_position.x, m_position.y); //Set position

	thor::FrameAnimation move, idle; //Our animations

	//Create our idle animation
	idle.addFrame(0.1f, sf::IntRect(0, 0, width, 42));

	//Create our move animation
	for (int i = 0; i < 6; i++)
	{
		move.addFrame(0.1f, sf::IntRect((i * width), 0, width, 42));
	}

	//Add our animations to our animator
	m_animator.addAnimation("Idle", idle, sf::seconds(1));
	m_animator.addAnimation("Moving", move, sf::seconds(0.7f));

	physics::world->addPhysicsBody(m_body); //Add to physics here, as we cannot modify the physics during a phjysics update
}

int Worker::randInt(int min, int max)
{
	std::random_device rd;  //Will be used to obtain a seed for the random number engine
	std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
	std::uniform_int_distribution<> dis(min, max);

	return dis(gen); //Generate random number between min and max (inclusive)
}
