#include "Player.h"

Player::Player(float x, float y) :
	m_position(x, y),
	m_moveSpeed(150.f), //Can move 150 pixels per second
	m_turnSpeed(85.f), //Can turn 85 degrees per second
	m_friction(.9990f),
	m_maxSpeed(240.0f),
	m_angle(-90),
	m_fireRate(.5f),
	m_timeToFire(m_fireRate),
	m_rangeCollider(x, y, 45),
	m_physicsBody(Type::Dynamic, Shape::Circle, this)
{
	setupAnimations(); //Setup our animations

	//Set our box parameters to our position and size and to NOT use gravity
	m_physicsBody.setCircleParameters(Vector2f(m_position.x, m_position.y), 30, 1.25f, false);
	//m_physicsBody.setBoxParameters(Vector2f(m_position.x, m_position.y), Vector2f(62, 85), 1, false);
	m_physicsBody.setFriction(m_friction);
	m_physicsBody.setRestitution(0.1f);
	m_physicsBody.mask = 1; //Set layer to 1, we can add 1 to other bitmasks on othe rbodies to filter out collisions with the player
	m_physicsBody.tag = "Player";

	//Add our body to our physics world
	physics::world->addPhysicsBody(m_physicsBody);

	//Pool 10 bullets
	for(int i = 0; i < 10; i++)
		m_bullets.push_back(PlayerBullet());
}

void Player::update(double dt)
{
	m_prevPos = m_physicsBody.position;

	m_timeToFire += dt; //Add to our fireTime

	//Update bullets
	for (auto& bullet : m_bullets)
		bullet.update(dt);

	m_dt = dt; //Set dt as we will use it in other places
	m_animator.update(sf::seconds(dt)); //Update our animator
	m_animator.animate(m_sprite); //Animate our sprite

	m_position = m_physicsBody.position; //Set our position to our physics body position

	//If our speed has gone past our max speed, clamp our speed to our max speed
	if (m_physicsBody.velocity.magnitude() > m_maxSpeed)
		m_physicsBody.velocity = m_physicsBody.velocity.normalise() * m_maxSpeed;

	//Set the position and rotation of our sprites and colliders
	m_sprite.setPosition(m_position.x, m_position.y);
	m_sprite.setRotation(m_angle + 90); //Add offset of 90 due to the sprite being faced the wrong way
	m_rangeCollider.setPosition(m_position);
}

void Player::draw(sf::RenderWindow & win, float a)
{
	//Draw bullets
	for (auto& bullet : m_bullets)
		bullet.draw(win);

	//Draw Player
	win.draw(m_sprite);
}

void Player::handleInput(InputHandler & input)
{
	float turnDir = 0; //The direction to turn - is left, + is right
	//Reset our movement variables
	m_isMoving = false, m_turningLeft = false, m_turningRight = false;
	m_turnVector.zeroVector(); //Reset our turn vector

	//If space was pressed and our gun is not recharging, spawn a bullet
	if (input.isButtonDown("Space") && m_timeToFire >= m_fireRate)
	{
		m_timeToFire = 0; //Reset our time to fire

		for (auto& bullet : m_bullets)
		{
			//If the bullet is not alive and currently colliding
			if (bullet.alive == false && bullet.collided == false)
			{
				bullet.spawn(m_position, m_angle);
				break;
			}
		}
	}

	//If moving in its current direction
	if (input.isButtonDown("W") || input.isButtonDown("Up"))
	{
		m_isMoving = true;

	}
	//If turning left
	if (input.isButtonDown("A") || input.isButtonDown("Left"))
	{
		turnDir -= m_turnSpeed * m_dt;
	}
	//If turning Right
	if (input.isButtonDown("D") || input.isButtonDown("Right"))
	{
		turnDir += m_turnSpeed * m_dt;
	}

	m_angle += turnDir; //Add our extra turn to our angle variable

	//If we are moving set our velocity & play our animation
	if (m_isMoving)
	{
		auto rad = thor::toRadian(m_angle); //Convert angle to a radian
		m_turnVector = Vector2f(cos(rad), sin(rad)); //Convert radian to a vector

		m_physicsBody.addForce(m_turnVector * m_moveSpeed * m_dt); //Add movement force to our physics body

		//Play our moving animation
		if (m_animator.isPlayingAnimation() && m_animator.getPlayingAnimation() != "Moving" || !m_animator.isPlayingAnimation())
		{
			m_animator.stopAnimation(); //Stop playing current animation
			m_animator.playAnimation("Moving", true); //Play our moving animation
		}

	}
	//Play our idle animation if we are not moving
	else if(m_animator.isPlayingAnimation() && m_animator.getPlayingAnimation() != "Idle" || !m_animator.isPlayingAnimation())
	{
		m_animator.stopAnimation(); //Stop playing current animation
		m_animator.playAnimation("Idle"); //Play our idle animationaw
	}
}

void Player::setTexture(ResourceManager & resources)
{
	m_sprite.setTexture(resources.getTexture("Player"));

	//Set center of our player sprite
	m_sprite.setOrigin(sf::Vector2f(31, 53));
	
	m_animator.playAnimation("Idle", true);

	//Set the bullet textures
	for (auto& bullet : m_bullets)
		bullet.setTexture(resources);
}

void Player::setupAnimations()
{
	thor::FrameAnimation move, idle; //Our animations

	//Create our idle animation
	idle.addFrame(0.1f, sf::IntRect(0,0, 62, 90));

	//Create our move animation
	for (int i = 0; i < 19; i++)
	{
		move.addFrame(0.1f, sf::IntRect(62 + (i * 62),0, 62, 90));
	}

	//Add our animations to our animator
	m_animator.addAnimation("Idle", idle, sf::seconds(1));
	m_animator.addAnimation("Moving", move, sf::seconds(0.33f));
}
