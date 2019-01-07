#include "GameScene.h"

GameScene::GameScene() :
	m_player(320, 160),
	test(Type::Static, Shape::Circle, this)
{
	m_followView.setSize(sf::Vector2f(1280, 720));
	m_minimapView.setSize(sf::Vector2f(1280 * 5, 720 * 5));
	m_minimapView.setCenter(sf::Vector2f(1280 * 5 / 2, 720 * 5 / 2));
	m_minimapView.setViewport(sf::FloatRect(0.005, 0.01, .25f, .25f));

	//Setup our BG colliders
	for (int i = 0; i < 5; i++) //5 rows
	{
		for (int j = 0; j < 5; j++) //5 Columns
		{
			m_bgColliders.push_back(sf::FloatRect(-3840 + (j * 3840), -2160 + (i * 2160), 3840, 2160));
		}
	}

	//Add our doors
	m_doors.push_back(Door(640, 160, m_player));
	m_doors.push_back(Door(1916, 160, m_player));

	test.setCircleParameters(Vector2f(320, 0), 100, 0, false);

	loadMap();

	//Add our body to our physics world
	physics::world->addPhysicsBody(test);
}

GameScene::~GameScene()
{
}

void GameScene::loadMap()
{
	//Loop through the map pieces and 
	for (auto& piece : m_levelLoader.data["Map Pieces"])
	{
		auto env = Environment(piece["X"], piece["Y"], piece["Tag"]);
		env.setRotation(piece["Angle"]); //Set the angle
		env.setScale(piece["Scale"][0], piece["Scale"][1]);
		//Create the environment and add it to our vector
		m_environment.push_back(env);
	}
}

void GameScene::createBoundary(json bounds, Environment & object)
{
	//Loop through the bounds and look for the boundaries tagged with objects tag
	for (auto& bound : bounds[object.tag])
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
		body->setBoxParameters(pos, size, 0, false); //Set the paaremeters of the body
		body->setInitialRotation(object.angle); //Set the initial rotation of the body
		physics::world->addPhysicsBody(*body); //Add body to the physics simulation
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

	//Draw our environment, rooms, corridors
	for (auto& object : m_environment)
	{
		//If the object is in view, then draw it
		if (object.collider().intersects(m_viewRect))
		{
			object.draw(window);
		}
	}

	//Draw our Doors
	for (auto& object : m_doors)
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

	//Set the windows view
	window.setView(m_followView);
}

void GameScene::drawMinimap(sf::RenderWindow & window)
{
	window.setView(m_minimapView);

	//Draw our bg sprite, everything else will be drawn over this
	for (auto& bg : m_bgColliders)
	{
		m_bgSprite.setPosition(bg.left, bg.top);
		window.draw(m_bgSprite);
	}

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

	for (auto& object : m_doors)
	{
		object.setTexture(resources);
	}

	//Get the boundaries information from the Json data
	json bounds = m_levelLoader.data["Boundaries"];

	//Set our environment textures and create our Boundaries for each piece
	for (auto& object : m_environment)
	{
		if (object.tag == "CTE") //Corner with Two Exits
		{
			object.setTexture(resources, "Top Left Corner Room");
		}
		else if (object.tag == "SC") //Straight Corridor
		{
			object.setTexture(resources, "Straight Corridor");
		}
		else if (object.tag == "3RE")
		{
			object.setTexture(resources, "Three Exit Room");
		}

		//Create the boundaries for the object
		createBoundary(bounds, object);
	}
}
