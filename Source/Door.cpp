#include "Door.h"

Door::Door(float x, float y, Player& player) :
	m_position(x, y),
	m_playerPtr(&player),
	m_closing(true),
	m_rangeCollider(x, y, 100)
{
	thor::FrameAnimation opening, closing;

	for (int i = 0; i < 18; i++)
	{
		opening.addFrame(.1f, sf::IntRect(i * 80, 0, 80, 160));
		closing.addFrame(.1f, sf::IntRect(1360 - (i * 80), 0, 80, 160));
	}

	m_animator.addAnimation("Opening", opening, sf::seconds(0.2f));
	m_animator.addAnimation("Closing", closing, sf::seconds(0.2f));
}

Door::~Door()
{
}

void Door::update(double dt)
{
	m_animator.update(sf::seconds(dt));
	m_animator.animate(m_sprite);

	//If the player is in range of the door, play the door opening animation
	if (Collisions::CircleVsCircle(m_rangeCollider, m_playerPtr->m_rangeCollider))
	{
		//If no animation is playing play our opening animation
		if (m_animator.isPlayingAnimation() && m_animator.getPlayingAnimation() != "Opening" || !m_animator.isPlayingAnimation() && m_closing)
		{
			m_animator.playAnimation("Opening", false);
			m_closing = false;
		}
	}
	else
	{
		//If no animation is playing play our opening animation
		if ((m_animator.isPlayingAnimation() && m_animator.getPlayingAnimation() != "Opening" || !m_animator.isPlayingAnimation()) && !m_closing)
		{
			m_animator.playAnimation("Closing", false);
			m_closing = true;
		}
	}
}

void Door::draw(sf::RenderWindow & win)
{
	win.draw(m_sprite);
}

void Door::setTexture(ResourceManager & resources)
{
	m_sprite.setTexture(resources.getTexture("Door"));
	m_sprite.setOrigin(sf::Vector2f(40, 80));
	m_sprite.setPosition(m_position.x, m_position.y);
	m_sprite.setTextureRect(sf::IntRect(0, 0, 80, 160));
}

void Door::setRotation(float angle)
{
	m_sprite.setRotation(angle); //Rotate the door sprite
}
