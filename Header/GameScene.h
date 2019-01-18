#pragma once
#include "LevelLoader.h" //For level data
#include "Scene.h"
#include "Player.h"
#include "Door.h"
#include "Environment.h" //For corridors, rooms
#include "Worker.h" //Worker Ai
#include "Sweeper.h"
#include "Predator.h" //Predator ships
#include "Nest.h" //Nest AI
#include "Grid.h" //For the grid of the map
#include "Hud.h" //For the hud

class GameScene : public Scene
{
public:
	GameScene();
	~GameScene();

	void loadMap();
	void createBoundary(json bounds, Environment& object);
	void update(double dt);
	void draw(sf::RenderWindow& window, float a);
	void drawMinimap(sf::RenderWindow& window);
	void handleInput(InputHandler& input);
	void setTexture(ResourceManager& resources);
private:
	Player m_player;
	sf::View m_followView;

	//Grid
	Grid m_grid;
	bool m_drawGrid, m_drawPhysics;

	//AI
	struct removeCapturedWorker
	{
		bool operator() (Worker& w) { return w.isCapturedByPlayer(); }
	};
	std::vector<Worker> m_workerAI; //All of our worker Ai's
	std::vector<Predator> m_predatorAI; //All of our predator Ai's

	Sweeper m_sweeperBot;

	//LevelLoader
	LevelLoader m_levelLoader;

	//Minimap
	sf::Sprite m_fullMapSprite;
	Hud m_hud;

	//Environment variables
	sf::Sprite m_bgSprite;
	std::vector<sf::FloatRect> m_bgColliders; //Our colliders for our BG, this will determine wheter to draw the bg sprite or not
	sf::FloatRect m_viewRect; //Rectangle for our view, we will use this to determine wheter to draw something or not
	std::vector<Environment> m_mapPieces; //Our vector for our map pieces, this will be our corridors and rooms
	std::vector<Environment> m_environment; //Our vector for our environment, this will be our spawn areas, our interior pices and such
	//std::vector<Environment> m_otherPieces; //Pices like worker areas, spawn areas and such
	std::vector<Door> m_doors; //Our vector of door,s there will be many doors throughout the map
};
