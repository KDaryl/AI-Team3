#include "Hud.h"

Hud::Hud(Player& player, Grid& grid) :
	m_playerPtr(&player),
	m_gridPtr(&grid),
	m_cleared(false),
	m_viewDetector(0, 0, 2880, 1620),
	m_maxWorkers(1),
	m_currentZoom(.25f),
	m_maxZoom(1.0f),
	m_zoomSpeed(.5f),
	m_zoomPercentage(0.25f),
	m_wayPointSet(false)
{
	m_rt.create(11520, 6480);
	m_sprite.setTexture(m_rt.getTexture());
	m_sprite.setOrigin(0, m_sprite.getGlobalBounds().height);
	m_sprite.setPosition(m_playerPtr->m_position.x, m_playerPtr->m_position.y);
	m_sprite.setScale(sf::Vector2f(.025f, -.025f));
	m_view = m_rt.getView();
	m_view.zoom(m_currentZoom);
	m_rt.setView(m_view);

	m_rect.setSize(sf::Vector2f(288, 162)); //.025% of size of the map (same size as minimap)
	m_rect.setFillColor(sf::Color::Transparent);
	m_rect.setOutlineColor(sf::Color::White);
	m_rect.setOutlineThickness(2.5f);

	updateHud();
}

void Hud::update(double dt)
{
	m_dt = dt;
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

	//Update our waypoint
	updateWaypoint(dt);
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
	m_defaultView = win.getView();

	m_sprite.setTexture(m_rt.getTexture());
	win.draw(m_rect);
	win.draw(m_sprite);
	win.draw(m_hudBg);
	win.draw(m_hpBarEmpty);
	win.draw(m_hpBarFull);
	win.draw(m_workerBarEmpty);
	win.draw(m_workerBarFull);

	//Draw waypoint if set
	if (m_wayPointSet)
	{
	}

	m_cleared = false;
}

void Hud::updateWaypoint(double dt)
{
	//Check if player has reached certain parts of the waypoint
	if (m_wayPointSet)
	{
		//If our path to the waypont is empty and the player is out of range of the point then get a path
		if (m_wayPointPath.size() == 0 && m_playerPtr->m_position.distance(m_waypointTarget) > 320)
		{

		}
	}
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

void Hud::handleInput(InputHandler & input)
{
	auto zoomCopy = m_currentZoom;

	//If left mouse is clicked
	if (input.isButtonPressed("LEFT_MOUSE"))
	{
		auto mouseR = sf::FloatRect((m_playerPtr->m_position.x - 640) + input.getMousePos().x, (m_playerPtr->m_position.y - 360) + input.getMousePos().y, 1,1);

		//If the player clicked on the minimap
		if (m_rect.getGlobalBounds().intersects(mouseR))
		{
			//Convert mouse to world coordinates as we want to get a waypoint to somewhere in the world, not on the view
			auto worldPos = m_rt.mapPixelToCoords(sf::Vector2i(input.getMousePos().x, input.getMousePos().y), m_view);
		}
	}

	//Increase minimap zoom
	if (input.isButtonDown("E"))
	{
		zoomCopy -= m_zoomSpeed * m_dt; //Zoom out
	}

	//Decrease minimap zoom
	else if (input.isButtonDown("Q"))
	{
		zoomCopy += m_zoomSpeed * m_dt; //Zoom out
	}

	//If we have zoomed in or out then 
	if (zoomCopy != m_currentZoom)
	{
		if (zoomCopy < .10f)
			zoomCopy = .10f;
		else if (zoomCopy > 1)
			zoomCopy = 1;


		m_zoomPercentage =  zoomCopy / m_maxZoom; //Get zoom percentage

		m_currentZoom = zoomCopy;

		auto zoomVec = sf::Vector2f(11520, 6480) * m_currentZoom;
		m_view.setSize(zoomVec);
		m_viewDetector.width = 11520 * zoomVec.x;
		m_viewDetector.height = 6480 * zoomVec.y;

		m_rt.setView(m_view);
	}
}
