#include "Game.h"

//updates per millisecond (60 fps)
static sf::Int32 MS_PER_UPDATE = 16;

Game::Game() :
	m_window(sf::VideoMode(1280, 720), "AI Project - Team 3")
{
	init();

	//Add our scenes to our scene manager
	m_sceneManager.addScene("Game Scene", &m_gameScene);

	//Set current scene
	m_sceneManager.setCurrentScene("Game Scene");

	//Once all scenes are added, set all the textures
	m_sceneManager.setTextures();
}

void Game::init()
{

}

void Game::run()
{
	sf::Clock clock;
	sf::Int32 lag = 0, fpsTimer = 0, frames = 0;
	while (m_window.isOpen())
	{
		processEvents();
		sf::Time dt = clock.restart();
		lag += dt.asMilliseconds();
		fpsTimer += dt.asMilliseconds();
		auto dtToSec = dt.asSeconds(); //Convert dt to seconds so we can avoid multiple calls to a method

		while (lag > MS_PER_UPDATE)
		{
			physics::world->update(dtToSec); //Update our physics
			update(dtToSec);
			lag -= MS_PER_UPDATE;
			frames++;
		}
		physics::world->update(dtToSec); //Update our physics
		update(dtToSec);
		render();

		//Checking FPS, this will bne useful for performance debugging
		if (fpsTimer >= 1000)
		{
			std::cout << "FPS: " << frames << std::endl;
			fpsTimer -= 1000;
			frames = 0;
		}
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
	m_window.clear(sf::Color::Blue); //Clear all previously drawn items

	m_sceneManager.draw(m_window); //Draw our current scene

	//Draw our physics colliders for debugging
	physics::world->draw(m_window);

	m_window.display(); //Display all drawn items
}

void Game::handleInput()
{
	//Update the input handler and see what buttons are pressed
	m_inputHandler.update(m_window);

	//Pass the input handler to the scnee manager
	m_sceneManager.handleInput(m_inputHandler);
}
