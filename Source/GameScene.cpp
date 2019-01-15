#include "GameScene.h"

GameScene::GameScene() :
	//player pointer maxSpeed and position
	m_sweeperBot(&m_player, 100, Vector2f(5760,6476)),
	m_player(5840, 6163),
	m_minimap(m_player)
{
	m_followView.setSize(sf::Vector2f(1280, 720));
	m_followView.zoom(1.0f);

	//Setup our BG colliders
	for (int i = 0; i < 5; i++) //5 rows
	{
		for (int j = 0; j < 5; j++) //5 Columns
		{
			m_bgColliders.push_back(sf::FloatRect(-3840 + (j * 3840), -2160 + (i * 2160), 3840, 2160));
		}
	}

	loadMap();
}

GameScene::~GameScene()
{
}

void GameScene::loadMap()
{
	//Loop through the map pieces and create our environment (rooms, corridors)
	for (auto& piece : m_levelLoader.data["Map Pieces"])
	{
		auto env = Environment(piece["X"], piece["Y"], piece["Tag"]);
		env.setRotation(piece["Angle"]); //Set the angle
		env.setScale(piece["Scale"][0], piece["Scale"][1]);
		//Create the environment and add it to our vector
		m_mapPieces.push_back(env);
	}

	//Load our doors
	for (auto& door : m_levelLoader.data["Doors"])
	{
		auto d = Door(door["X"], door["Y"], m_player);
		d.setRotation(door["Angle"]);
		m_doors.push_back(d);
	}

	//Load worker Areas
	for (auto& area : m_levelLoader.data["Worker Areas"])
	{
		auto env = Environment(area["X"], area["Y"], "Worker Area");
		m_environment.push_back(env);
		m_workerAI.push_back(Worker(Vector2f(area["X"], area["Y"]), &m_grid));
	}
	//Load our spawn point
	for (auto& spawn : m_levelLoader.data["Spawn Point"])
	{
		m_environment.push_back(Environment(spawn["X"], spawn["Y"], "Spawn Point"));
	}
}

void GameScene::createBoundary(json bounds, Environment & object)
{
	//Loop through the bounds and look for the boundaries tagged with objects tag
	for (auto& bound : bounds[object.tag]["Walls"])
	{
		auto body = new PhysicsBody(Type::Static, Shape::Box, this); //Create the body
		auto size = Vector2f(bound["W"], bound["H"]);
		auto pos = Vector2f(bound["X"], bound["Y"]); //Center the physics body
		pos.x += object.m_position.x;
		pos.y += object.m_position.y;
		if (object.angle != 0)
		{
			sf::Transform tf;//Rotate position around the center of the objects position
			tf.rotate(object.angle, sf::Vector2f(object.m_position.x, object.m_position.y));
			auto posAfter = tf.transformPoint(sf::Vector2f(pos.x, pos.y));
			pos = Vector2f(posAfter.x, posAfter.y);
		}
		body->setBoxParameters(pos, size, 0, false); //Set the parameters of the body
		body->setInitialRotation(object.angle); //Set the initial rotation of the body
		physics::world->addPhysicsBody(*body); //Add body to the physics simulation
	}

	//Loop through the safe Areas for the object
	for (auto& bound : bounds[object.tag]["Safe Areas"])
	{
		auto size = Vector2f(bound["W"], bound["H"]);
		auto pos = Vector2f(bound["X"], bound["Y"]); //Center of the collision box
		pos.x += object.m_position.x;
		pos.y += object.m_position.y;
		CollisionBox safeArea(pos.x, pos.y, size.x, size.y); //Create the collision box

		if (object.angle != 0) //If the object is rotated, rotate our collision box
		{
			sf::Transform tf;//Rotate position around the center of the objects position
			tf.rotate(object.angle, sf::Vector2f(object.m_position.x, object.m_position.y));
			auto posAfter = tf.transformPoint(sf::Vector2f(pos.x, pos.y));
			pos = Vector2f(posAfter.x, posAfter.y); //Set the new position
			safeArea.setSize(pos.x, pos.y, size.x, size.y);
			safeArea.rotate(object.angle);
		}

		//Loop through all of the Big Cells and see where our object is
		for (auto& cells :	m_grid.m_splitGridcells)
		{
			//If the Safe Area intersects with with the grid, go mark the cells it collides with as not a wall
			if (safeArea.rect.getGlobalBounds().intersects(cells.second.rect))
			{
				std::string gridP = std::to_string(cells.second.gridPosition.x) + "," + std::to_string(cells.second.gridPosition.y);

				for (auto& smallCell : m_grid.m_splitCells[gridP])
				{
					if (smallCell->rect.intersects(safeArea.rect.getGlobalBounds()))
					{
						smallCell->isWall = false;
					}
				}
			}
		}
	}
}


void GameScene::update(double dt)
{
	//Update the doors on the map
	for (auto& object : m_doors)
	{
		object.update(dt);
	}

	//Set the views position to follow the player (player will be centered)
	m_followView.setCenter(m_player.m_position.x, m_player.m_position.y);
	m_viewRect = sf::FloatRect(m_player.m_position.x - 640, m_player.m_position.y - 360, 1280, 720);

	//Update minimap
	m_minimap.update();

	//Update Player
	m_player.update(dt);

	//Update workers
	for (auto& worker : m_workerAI)
	{
		worker.update(dt);

		//If the circles collide set the worker to follow the Sweeper Bot
		if (Collisions::CircleVsCircle(worker.rangeCollider(), m_player.m_rangeCollider))
		{
			//If the worker is not captured, set it as captured
			if (!worker.captured())
			{
				worker.captureWorker(false, m_player.m_position);
			}
		}
	}
	//update ai
	m_sweeperBot.update(dt);
}

void GameScene::draw(sf::RenderWindow & window, float a)
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

	//Draw the map
	window.draw(m_fullMapSprite);

	//Draw our Doors
	for (auto& object : m_doors)
	{
		//If the object is in view, then draw it
		if (object.collider().intersects(m_viewRect))
		{
			object.draw(window);
		}
	}

	//Draw environment
	for (auto& object : m_environment)
	{
		//If the object is in view, then draw it
		if (object.collider().intersects(m_viewRect))
		{
			object.draw(window);
		}
	}

	//Draw workers
	for (auto& worker : m_workerAI)
	{
		worker.draw(window);
	}

	//Draw the player
	m_player.draw(window, a);

	//draw the ai
	m_sweeperBot.render(window);

	//The Grid
	if(m_drawGrid)
		m_grid.draw(window);

	//Draw our physics colliders for debugging
	if (m_drawPhysics)
		physics::world->draw(window);

	drawMinimap(window); //Draw the mini map

	//Set the windows view
	window.setView(m_followView);
}

void GameScene::drawMinimap(sf::RenderWindow & window)
{
	m_minimap.draw(m_fullMapSprite);

	//Draw environment
	for (auto& object : m_environment)
	{
		//If the object is in view, then draw it
		if (object.collider().intersects(m_minimap.getViewDetector()))
		{
			m_minimap.draw(object.m_sprite);
		}
	}

	m_minimap.draw(m_player.m_sprite);
	m_minimap.display(window);
}

void GameScene::handleInput(InputHandler & input)
{
	m_player.handleInput(input);

	//Keybindings for turning Grid and Collision boxes On/OFF
	if (input.isButtonPressed("Shift"))
		m_drawGrid = !m_drawGrid;
	if (input.isButtonPressed("Tab"))
		m_drawPhysics = !m_drawPhysics;
}

void GameScene::setTexture(ResourceManager & resources)
{
	m_player.setTexture(resources);
	//as AI is a base class the specific texture will need to be told here
	m_sweeperBot.setTexture(resources, "Sweeper", Vector2f(18, 9.5));

	m_bgSprite.setTexture(resources.getTexture("Starfield BG"));

	for (auto& object : m_doors)
	{
		object.setTexture(resources);
	}

	m_fullMapSprite.setTexture(resources.getTexture("Full Map"));

	//Get the boundaries information from the Json data
	json bounds = m_levelLoader.data["Boundaries"];

	//Set our environment textures
	for (auto& object : m_environment)
	{
		//Set the texture of the object
		object.setTexture(resources, object.tag);
	}

	//Set our map piece textures and create our Boundaries for each piece
	for (auto& object : m_mapPieces)
	{
		//Set the texture of the object
		object.setTexture(resources, object.tag);

		//Create the boundaries for the object
		createBoundary(bounds, object);
	}

	//Set workers textures
	for (auto& worker : m_workerAI)
	{
		worker.setTexture(resources);
	}
}
