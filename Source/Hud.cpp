#include "Hud.h"

Hud::Hud(Player& player) :
	m_playerPtr(&player),
	m_cleared(false),
	m_viewDetector(0, 0, 2880, 1620),
	m_maxWorkers(1)
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

	updateHud();
}

void Hud::update()
{
	updateHud(); //Update HUD elements

	m_viewDetector.left = m_playerPtr->m_position.x - m_viewDetector.width / 2;
	m_viewDetector.top = m_playerPtr->m_position.y - m_viewDetector.height / 2;

	m_sprite.setPosition(m_playerPtr->m_position.x - 630, m_playerPtr->m_position.y - 350);
	m_view.setCenter(m_playerPtr->m_position.x, m_playerPtr->m_position.y);
	m_rect.setPosition(m_playerPtr->m_position.x - 630, m_playerPtr->m_position.y - 350);

	//Set sprite positions
	m_hudBg.setPosition((m_playerPtr->m_position.x - 640) + 155, (m_playerPtr->m_position.y - 360) + 200);
	m_hpBarEmpty.setPosition((m_playerPtr->m_position.x - 640) + 223, (m_playerPtr->m_position.y - 360) + 190);
	m_hpBarFull.setPosition((m_playerPtr->m_position.x - 640) + 223, (m_playerPtr->m_position.y - 360) + 190);
	m_workerBarEmpty.setPosition((m_playerPtr->m_position.x - 640) + 223, (m_playerPtr->m_position.y - 360) + 210);
	m_workerBarFull.setPosition((m_playerPtr->m_position.x - 640) + 223, (m_playerPtr->m_position.y - 360) + 210);
}

void Hud::draw(sf::Drawable & item)
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

void Hud::display(sf::RenderWindow & win)
{
	m_sprite.setTexture(m_rt.getTexture());
	win.draw(m_rect);
	win.draw(m_sprite);
	win.draw(m_hudBg);
	win.draw(m_hpBarEmpty);
	win.draw(m_hpBarFull);
	win.draw(m_workerBarEmpty);
	win.draw(m_workerBarFull);

	m_cleared = false;
}

void Hud::setTexture(ResourceManager & resources)
{
	m_hudBg.setTexture(resources.getTexture("HUD"));
	m_hpBarEmpty.setTexture(resources.getTexture("HUD Bar Empty"));
	m_hpBarFull.setTexture(resources.getTexture("HUD Bar Full"));
	m_workerBarEmpty.setTexture(resources.getTexture("HUD Bar Empty"));
	m_workerBarFull.setTexture(resources.getTexture("HUD Bar Full"));

	//Center sprites
	m_hudBg.setOrigin(150, 20);
	m_hpBarEmpty.setOrigin(76, 6);
	m_hpBarFull.setOrigin(76, 6);
	m_workerBarEmpty.setOrigin(76, 6);
	m_workerBarFull.setOrigin(76, 6);

	m_hpBarFull.setTextureRect(sf::IntRect(0, 0, m_hpBarFull.getLocalBounds().width * m_hpPercent, m_hpBarFull.getLocalBounds().height));
	m_workerBarFull.setTextureRect(sf::IntRect(0, 0, m_workerBarFull.getLocalBounds().width * m_workerPercent, m_workerBarFull.getLocalBounds().height));
}

void Hud::updateHud()
{
	m_hpPercent = (float)m_playerPtr->health / m_playerPtr->maxHealth; //Health Percentage
	m_workerPercent = (float)m_playerPtr->workersCollected / m_maxWorkers; // Worker percentage

	m_hpBarFull.setTextureRect(sf::IntRect(0,0, 152 * m_hpPercent, 12));
	m_workerBarFull.setTextureRect(sf::IntRect(0, 0, 152 * m_workerPercent, 12));
}