#include "EnemyBullet.h"

EnemyBullet::EnemyBullet() :
	alive(false),
	collided(false),
	dead(false),
	m_speed(725),
	m_body(Type::Static, Shape::Circle, this)
{
	m_body.setCircleParameters(m_position, 5, 1, false);
	m_body.tag = "Predator Bullet";
	m_body.mask = 3;
	m_body.mass = 1;
	m_body.bitmasks.push_back(3); //Avoid collisions with predator ships and enemy bullets
	m_body.isSensor = true; //Is a sensor, means it will avouid collision resolution (wont bounce)
}

/**
* Description: Updates the enemy bullet
*/
void EnemyBullet::update(double dt)
{
	//Update animator
	m_animator.update(sf::seconds(dt));
	m_animator.animate(m_sprite);

	//If alive, play our moving animation
	if (alive)
	{
		//Play our moving animation
		if (m_animator.isPlayingAnimation() && m_animator.getPlayingAnimation() != "Move" || !m_animator.isPlayingAnimation())
		{
			m_animator.stopAnimation(); //Stop playing current animation
			m_animator.playAnimation("Move", true); //Play our moving animation
		}

		//Set the sprites position to the bodies position
		m_sprite.setPosition(m_body.position.x, m_body.position.y);
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

/**
* Description: Draws the bullet
*/
void EnemyBullet::draw(sf::RenderWindow & window)
{
	//Only draw if the animations are playing
	if (alive || collided)
	{
		window.draw(m_sprite);
	}
}

/**
* Description: Spawns the bullet at the position adn at the angle passed
*/
void EnemyBullet::spawn(Vector2f position, float angle)
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
	m_body.addForce(m_velocity * m_speed); //Add our velocity to our body

	//Play our moving animation
	if (m_animator.isPlayingAnimation() && m_animator.getPlayingAnimation() != "Move" || !m_animator.isPlayingAnimation())
	{
		m_animator.stopAnimation(); //Stop playing current animation
		m_animator.playAnimation("Move", true); //Play our moving animation
	}
}

void EnemyBullet::setTexture(ResourceManager & resources)
{
	m_sprite.setTexture(resources.getTexture("Predator Bullet"));
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
* Description: Sets the bullet as collided 
*/
void EnemyBullet::hasCollided()
{
	alive = false;
	collided = true;
	m_body.position.zeroVector();
	m_body.velocity.zeroVector();
	m_body.type = Type::Static; //Change to static as we dont want physics to take over
}
