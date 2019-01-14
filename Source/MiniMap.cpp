#include "MiniMap.h"

MiniMap::MiniMap(Player& player) :
	m_playerPtr(&player),
	m_cleared(false)
{
	m_rt.create(11520, 6480);
	m_sprite.setTexture(m_rt.getTexture());
	m_sprite.setOrigin(0, m_sprite.getGlobalBounds().height);
	m_sprite.setPosition(m_playerPtr->m_position.x, m_playerPtr->m_position.y);
	m_sprite.setScale(sf::Vector2f(.025f, -.025f));
	m_view = m_rt.getView();
	m_view.zoom(.25f);
	m_rt.setView(m_view);

	m_rect.setSize(sf::Vector2f(288, 162)); //.025% of size of the map (same size as minimap)
	m_rect.setFillColor(sf::Color::Transparent);
	m_rect.setOutlineColor(sf::Color::White);
	m_rect.setOutlineThickness(2.5f);
}

void MiniMap::update()
{
	m_sprite.setPosition(m_playerPtr->m_position.x - 630, m_playerPtr->m_position.y - 350);
	m_view.setCenter(m_playerPtr->m_position.x, m_playerPtr->m_position.y);
	m_rect.setPosition(m_playerPtr->m_position.x - 630, m_playerPtr->m_position.y - 350);
}

void MiniMap::draw(sf::Drawable & item)
{
	//If the texture wasnt cleared, clear it with black
	if (!m_cleared)
	{
		m_rt.setView(m_view);
		m_rt.clear(sf::Color::Black);
		m_cleared = true;
	}

	m_rt.draw(item);
}

void MiniMap::display(sf::RenderWindow & win)
{
	m_sprite.setTexture(m_rt.getTexture());
	win.draw(m_rect);
	win.draw(m_sprite);
	m_cleared = false;
}