#include "Game.h"

//updates per millisecond (60 fps)
static sf::Int32 MS_PER_UPDATE = 16;

Game::Game() :
	m_window(sf::VideoMode(1280, 720), "AI Project - Team 3"),
	m_loadingThread(&Game::loadingScreen, this)
{
	init();

	/// Start of our thread code, everything must be executed in this order for the
	/// loading screen to work correctly
	m_window.setActive(false);

	//Start Thread for loading screen
	m_loadingThread.launch();

	//Load textures
	m_sceneManager.m_resources.loadTextures();

	m_loadingThread.wait(); //Block execution of code until the thread finishes

	m_window.setActive(true);
	///End of thread code

	//Add our scenes to our scene manager
	m_sceneManager.addScene("Game Scene", &m_gameScene);

	//Set current scene
	m_sceneManager.setCurrentScene("Game Scene");

	//Once all scenes are added, set all the textures
	m_sceneManager.setTextures();
}

void Game::init()
{
	//m_window.setFramerateLimit(60);
}

void Game::run()
{
	sf::Clock clock;
	float lag = 0;
	float physStep = 1 / 60.f;
	while (m_window.isOpen())
	{
		processEvents();
		sf::Time dt = clock.restart();
		auto dtToSec = dt.asSeconds(); //Convert dt to seconds so we can avoid multiple calls to a method
		lag += dtToSec; //Add to our lag 
		physics::world->update(dtToSec); //Update our physics
		//If lag accumalated is greater than physicsStep
		while (lag > physStep)
		{
			physics::dt = lag;
			physics::world->checkCollision();
			lag -= physStep;
		}
		update(dtToSec);
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
	m_fps.update(); //Update FPS

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

void Game::loadingScreen()
{
	m_window.setActive(true);

	sf::Clock clock;
	sf::Time dt;

	thor::FrameAnimation load;

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 30; j++)
		{
			load.addFrame(0.1f, sf::IntRect(j * 420, i * 420, 420, 420));
		}
	}
	m_loadAnimator.addAnimation("Load", load, sf::seconds(1));

	m_loadingSprite.setTexture(m_sceneManager.m_resources.getTexture("Loading Screen"));
	m_loadingSprite.setPosition(430, 150);

	m_loadAnimator.playAnimation("Load", true); //Play Animation

	while (m_sceneManager.m_resources.finishedLoading == false)
	{
		dt = clock.restart(); //Restart the clock

		//Update loading animation
		m_loadAnimator.update(sf::seconds(dt.asSeconds())); //Update our animator
		m_loadAnimator.animate(m_loadingSprite); //Animate our sprite

		//Draw loading animation
		m_window.clear(sf::Color::Black); //Clear with black

		m_window.draw(m_loadingSprite); //Draw the loading screen sprite/animation

		m_window.display(); //Show loading screen
	}

	m_window.clear(); //Clear the window

	m_window.setActive(false);

	system("CLS"); //Clear the debug
}
