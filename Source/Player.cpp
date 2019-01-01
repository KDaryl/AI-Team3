#include "Player.h"

Player::Player() :
	m_position(320, 160),
	m_moveSpeed(.2f),
	m_turnSpeed(.075f),
	m_friction(.9997f),
	m_rangeCollider(0,0, 186, 255),
	m_physicsBody(Type::Dynamic, Shape::Box, this)
{
	setupAnimations(); //Setup our animations

	//Set our box parameters to our position and size and to NOT use gravity
	m_physicsBody.setBoxParameters(Vector2f(m_position.x, m_position.y), Vector2f(62, 85), false);
	m_physicsBody.setFriction(m_friction);

	//Add our body to our physics world
	physics::world->addPhysicsBody(m_physicsBody);
}

void Player::update(double dt)
{
	m_dt = dt; //Set dt as we will use it in other places
	m_animator.update(sf::seconds(dt)); //Update our animator
	m_animator.animate(m_sprite); //Animate our sprite

	m_position = m_physicsBody.position; //Set our position to our physics body position

	//Set the position and rotation of our sprites and colliders
	m_sprite.setPosition(m_position.x, m_position.y);
	m_sprite.setRotation(m_angle + 90); //Add offset of 90 due to the sprite being faced the wrong way
	m_rangeCollider.left = m_position.x - m_rangeCollider.width / 2;
	m_rangeCollider.top = m_position.y - m_rangeCollider.height / 2;
}

void Player::draw(sf::RenderWindow & win)
{
	win.draw(m_sprite);
}

void Player::handleInput(InputHandler & input)
{
	float turnDir = 0; //The direction to turn - is left, + is right
	//Reset our movement variables
	m_isMoving = false, m_turningLeft = false, m_turningRight = false;
	m_turnVector.zeroVector(); //Reset our turn vector

	//If moving in its current direction
	if (input.isButtonDown("W") || input.isButtonDown("Up"))
	{
		m_isMoving = true;

	}
	//If turning left
	if (input.isButtonDown("A") || input.isButtonDown("Left"))
	{
		turnDir -= m_turnSpeed;
	}
	//If turning Right
	if (input.isButtonDown("D") || input.isButtonDown("Right"))
	{
		turnDir += m_turnSpeed;
	}

	m_angle += turnDir; //Add our extra turn to our angle variable

	//If we are moving set our velocity & play our animation
	if (m_isMoving)
	{
		auto rad = thor::toRadian(m_angle); //Convert angle to a radian
		m_turnVector = Vector2f(cos(rad), sin(rad)); //Convert radian to a vector

		m_turnVector = m_turnVector.normalise();

		m_physicsBody.velocity += m_turnVector * m_moveSpeed * m_dt; //Calculate our speed

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
	m_sprite.setOrigin(sf::Vector2f(31, 50));
	
	m_animator.playAnimation("Idle", true);
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
