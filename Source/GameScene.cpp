#include "GameScene.h"

GameScene::GameScene() :
	m_player(5840, 6163),
	m_seekAI(Vector2f(5760, 6576), 0.1),
	m_fleeAI(Vector2f(5760, 6576), 0.1),
	m_wanderAI(Vector2f(5760, 6576),0.1)
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

	m_miniMapTexture.create(11520, 6480);
	m_miniMapSprite.setTexture(m_miniMapTexture.getTexture());
	m_miniMapSprite.setOrigin(0, m_miniMapSprite.getGlobalBounds().height);
	m_miniMapSprite.setPosition(m_player.m_position.x, m_player.m_position.y);
	m_miniMapSprite.setScale(sf::Vector2f(.025f, -.025f));
	m_miniMapView = m_miniMapTexture.getView();
	m_miniMapView.zoom(.25f);
	m_miniMapTexture.setView(m_miniMapView);
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
		m_environment.push_back(env);
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
	}
	//Load our spawn point
	for(auto& spawn : m_levelLoader.data["Spawn Point"])
		m_environment.push_back(Environment(spawn["X"], spawn["Y"], "Spawn Point"));
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
		body->setBoxParameters(pos, size, 0, false); //Set the parameters of the body
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
	m_miniMapSprite.setPosition(m_player.m_position.x - 630, m_player.m_position.y - 350);
	m_miniMapView.setCenter(m_player.m_position.x, m_player.m_position.y);

	//Update Player
	m_player.update(dt);
	//update to seek player position
	m_seekAI.update(Vector2f(m_player.m_position.x, m_player.m_position.y));
	m_fleeAI.update(Vector2f(m_player.m_position.x, m_player.m_position.y));
	m_wanderAI.update(Vector2f(m_player.m_position.x, m_player.m_position.y));
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

	//Draw the player
	m_player.draw(window);
	//draw the ai
	m_seekAI.render(window);
	m_fleeAI.render(window);
	m_wanderAI.render(window);

	drawMinimap(window); //Draw the mini map

	//Set the windows view
	window.setView(m_followView);
}

void GameScene::drawMinimap(sf::RenderWindow & window)
{
	//Set the minimap view
	m_miniMapTexture.setView(m_miniMapView);

	//Clear the minimap with black
	m_miniMapTexture.clear(sf::Color::Black);

	//Draw the whole background image
	m_miniMapTexture.draw(m_fullMapSprite);

	m_miniMapTexture.draw(m_player.m_sprite);

	//m_miniMapTexture.
	m_miniMapSprite.setTexture(m_miniMapTexture.getTexture());

	window.draw(m_miniMapSprite);
}

void GameScene::handleInput(InputHandler & input)
{
	m_player.handleInput(input);
}

void GameScene::setTexture(ResourceManager & resources)
{
	m_player.setTexture(resources);
	//as AI is a base class the specific texture will need to be told here
	m_seekAI.setTexture(resources, "Sweeper", Vector2f(18, 9.5));
	m_fleeAI.setTexture(resources, "Sweeper", Vector2f(18, 9.5));
	m_wanderAI.setTexture(resources, "Sweeper", Vector2f(18, 9.5));
	m_bgSprite.setTexture(resources.getTexture("Starfield BG"));

	for (auto& object : m_doors)
	{
		object.setTexture(resources);
	}

	m_fullMapSprite.setTexture(resources.getTexture("Full Map"));

	//Get the boundaries information from the Json data
	json bounds = m_levelLoader.data["Boundaries"];

	//Set our environment textures and create our Boundaries for each piece
	for (auto& object : m_environment)
	{
		//Set the texture of the object
		object.setTexture(resources, object.tag);

		//Create the boundaries for the object
		createBoundary(bounds, object);
	}
}
