#pragma once
#include "SFML/Graphics.hpp"
#include "InputHandler.h"
#include "SceneManager.h"
#include "LevelLoader.h"
using namespace physics;

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

	//LevelLoader
	LevelLoader m_levelLoader;

	//Scene variables
	SceneManager m_sceneManager;
	GameScene m_gameScene;
};