#include "Environment.h"

Environment::Environment(std::string _tag, float x, float y) :
	m_position(x, y),
	tag(_tag)
{
}

Environment::~Environment()
{
}

void Environment::draw(sf::RenderWindow & win)
{
	win.draw(m_sprite);
}

void Environment::setTexture(ResourceManager & resources, std::string textureName)
{
	m_sprite.setTexture(resources.getTexture(textureName)); //Set texture
	m_sprite.setOrigin(m_sprite.getGlobalBounds().width / 2, m_sprite.getGlobalBounds().height / 2); //Set Origin
	m_sprite.setPosition(m_position.x, m_position.y);

	//Set our collider
	m_collider = sf::FloatRect(m_position.x - m_sprite.getGlobalBounds().width / 2, m_position.y - m_sprite.getGlobalBounds().height / 2, m_sprite.getGlobalBounds().width, m_sprite.getGlobalBounds().height);
}

void Environment::setRotation(float angle)
{
	m_sprite.setRotation(angle);
}

void Environment::setScale(float x, float y)
{
	m_sprite.setScale(x, y);
}
