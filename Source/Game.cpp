#include "Game.h"

//updates per millisecond
static sf::Int32 MS_PER_UPDATE = 10;

Game::Game() :
	m_window(sf::VideoMode(1280, 720), "AI Project - Team 3")
{
	init();
}

void Game::init()
{

}

void Game::run()
{
	sf::Clock clock;
	sf::Int32 lag = 0;
	while (m_window.isOpen())
	{
		processEvents();
		sf::Time dt = clock.restart();
		lag += dt.asMilliseconds();


		while (lag > MS_PER_UPDATE)
		{
			update(dt.asSeconds());
			lag -= MS_PER_UPDATE;
		}
		update(dt.asSeconds());
		render();
	}
}

void Game::processEvents()
{
	sf::Event event;
	while (m_window.pollEvent(event))
	{
		// check the type of the event...
		switch (event.type)
		{
		// window closed
		case sf::Event::Closed:
			m_window.close();
			break;

		// we don't process other types of events
		default:
			break;
		}
	}
}

void Game::update(double dt)
{
	handleInput(); //Handle any input events in the game

	m_sceneManager.update(dt);
}

void Game::render()
{
	m_window.clear(sf::Color::Black); //Clear all previously drawn items

	m_sceneManager.draw(m_window); //Draw our current scene

	m_window.display(); //Display all drawn items
}

void Game::handleInput()
{
	m_inputHandler.update(m_window);
}
