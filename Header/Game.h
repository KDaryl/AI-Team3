#pragma once
#include "SFML/Graphics.hpp"
#include "InputHandler.h"
#include "SceneManager.h"
using namespace physics;

//FPS Counter
class FPS
{
public:
	unsigned int mFrame;
	unsigned int mFps;
	sf::Clock mClock;

	FPS() : mFrame(0), mFps(0) { mClock.restart(); }//Start FPS Clock

	void update()
	{
		if (mClock.getElapsedTime().asSeconds() >= 1.f)
		{
			mFps = mFrame / 2;
			mFrame = 0;
			mClock.restart();

			std::cout << mFps << std::endl;
		}
		++mFrame;
	}
};

//Game loop
class Game
{
public:
	Game();
	void run();
	void init();
	void processEvents();
private:
	void update(double dt);
	void render();
	void handleInput(); //Handles any input for our game

	sf::RenderWindow m_window; //Render window
	InputHandler m_inputHandler;

	//Scene variables
	SceneManager m_sceneManager;
	GameScene m_gameScene;

	FPS m_fps; //Our FPS Counter
};
