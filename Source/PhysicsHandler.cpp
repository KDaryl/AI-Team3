#include "PhysicsHandler.h"

PhysicsHandler::PhysicsHandler()
{
}

PhysicsHandler::~PhysicsHandler()
{
}

void PhysicsHandler::update(float dt)
{
	//Loop through our bodies
	for (auto& body : physics::world->bodies)
	{
		//If our body uses gravity, add gravity to our bodies velocity
		if (body->useGravity)
			body->applyGravity(physics::world->gravity * dt);

		//Update the body
		body->update(dt);
	}
}

void PhysicsHandler::draw(sf::RenderWindow & window)
{
	sf::RectangleShape rect;
	rect.setFillColor(sf::Color::Transparent);
	rect.setOutlineThickness(7.5f);
	rect.setOutlineColor(sf::Color::Red);

	sf::CircleShape circle;
	circle.setFillColor(sf::Color::Transparent);
	circle.setOutlineThickness(7.5f);
	circle.setOutlineColor(sf::Color::Red);

	//Loop through our bodies
	for (auto& body : physics::world->bodies)
	{
		if (body->shape == Shape::Box)
		{
			rect.setSize(sf::Vector2f(body->size.x, body->size.y));
			rect.setPosition(body->position.x, body->position.y);
			window.draw(rect);
		}
		else
		{
			circle.setRadius(body->radius);
			circle.setPosition(body->position.x, body->position.y);
			window.draw(circle);
		}
	}
}

void PhysicsHandler::addPhysicsBody(PhysicsBody & body)
{
	physics::world->bodies.push_back(&body); //Add the body to the vector of physics bodies
}

void PhysicsHandler::deletePhysicsBody(PhysicsBody & body)
{
	physics::world->bodies.erase(std::remove_if(physics::world->bodies.begin(), physics::world->bodies.end(),
		[body](PhysicsBody* i) {return i && (*i == body); }));
}