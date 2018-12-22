#include "GameScene.h"

GameScene::GameScene()
{
	m_followView.setSize(sf::Vector2f(1280, 720));
	m_minimapView.setSize(sf::Vector2f(1280 * 5, 720 * 5));
	m_minimapView.setCenter(sf::Vector2f(1280 * 5 / 2, 720 * 5 / 2));
	m_minimapView.setViewport(sf::FloatRect(0, 0, .25f, .25f));

	//Setup our BG colliders
	for (int i = 0; i < 5; i++) //5 rows
	{
		for (int j = 0; j < 5; j++) //5 Columns
		{
			m_bgColliders.push_back(sf::FloatRect(-3840 + (j * 3840), -2160 + (i * 2160), 3840, 2160));
		}
	}


	//Add objects to our environment, this will have to be loaded from a text file of some sort, yaml?
	m_environment.push_back(Environment("Top Left Corner", 320, 160));
	m_environment.push_back(Environment("Straight Corridor Y Flipped", 958, 160));
}

GameScene::~GameScene()
{
}

void GameScene::update(double dt)
{
	//Set the views position to follow the player (player will be centered)
	m_followView.setCenter(m_player.m_position.x, m_player.m_position.y);
	m_viewRect = sf::FloatRect(m_player.m_position.x - 640, m_player.m_position.y - 360, 1280, 720);

	m_player.update(dt);
}

void GameScene::draw(sf::RenderWindow & window)
{
	//Set the windows view
	window.setView(m_followView);

	//Draw our bg sprite, everything else will be drawn over this
	for (auto& bg : m_bgColliders)
	{
		if (bg.intersects(m_viewRect))
		{
			m_bgSprite.setPosition(bg.left, bg.top);
			window.draw(m_bgSprite);
		}
	}

	//Draw our environment, rooms, corridors, doors etc
	for (auto& object : m_environment)
	{
		//If the object is in view, then draw it
		if (object.collider().intersects(m_viewRect))
		{
			object.draw(window);
		}
	}

	//Draw the player
	m_player.draw(window);

	drawMinimap(window); //Draw the mini map
}

void GameScene::drawMinimap(sf::RenderWindow & window)
{
	window.setView(m_minimapView);

	//Draw all of our environment objects
	for (auto& obj : m_environment)
	{
		obj.draw(window);
	}

	//Draw out players marker point (just the player for now)
	m_player.draw(window);
}

void GameScene::handleInput(InputHandler & input)
{
	m_player.handleInput(input);
}

void GameScene::setTexture(ResourceManager & resources)
{
	m_player.setTexture(resources);

	m_bgSprite.setTexture(resources.getTexture("Starfield BG"));

	//Set our environment textures
	for (auto& object : m_environment)
	{
		if (object.tag == "Top Left Corner")
		{
			object.setTexture(resources, "Top Left Corner Room");
		}
		else if (object.tag == "Straight Corridor")
		{
			object.setTexture(resources, "Straight Corridor");
		}
		else if (object.tag == "Straight Corridor Y Flipped")
		{
			object.setTexture(resources, "Straight Corridor");
			object.setScale(1, -1);
		}
		//Add more tag checks, if a room is a corner rotated, then set rotation
	}
}
