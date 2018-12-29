#pragma once
#include "Scene.h"
#include "Player.h"
#include "Door.h"
#include "Environment.h" //For corridors, rooms

class GameScene : public Scene
{
public:
	GameScene();
	~GameScene();

	void update(double dt);
	void draw(sf::RenderWindow& window);
	void drawMinimap(sf::RenderWindow& window);
	void handleInput(InputHandler& input);
	void setTexture(ResourceManager& resources);
private:
	Player m_player;
	sf::View m_followView, m_minimapView;

	//Environment variables
	sf::Sprite m_bgSprite;
	std::vector<sf::FloatRect> m_bgColliders; //Our colliders for our BG, this will determine wheter to draw the bg sprite or not
	sf::FloatRect m_viewRect; //Rectangle for our view, we will use this to determine wheter to draw something or not
	std::vector<Environment> m_environment; //Our vector for our environment, this will be our corridors and rooms
	std::vector<Door> m_doors; //Our vector of door,s there will be many doors throughout the map
};

