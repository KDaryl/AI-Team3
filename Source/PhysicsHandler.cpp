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
	for (auto& body : bodies)
	{
		//If our body uses gravity, add gravity to our bodies velocity
		if (body->useGravity)
			body->applyGravity(PhysicsHandler::gravity * dt);

		//Update the body
		body->update(dt);
	}
}

void PhysicsHandler::draw(sf::RenderWindow & window)
{
	sf::RectangleShape rect;
	rect.setFillColor(sf::Color::Transparent);
	rect.setOutlineThickness(2.5f);
	rect.setOutlineColor(sf::Color::Red);

	sf::CircleShape circle;
	circle.setFillColor(sf::Color::Transparent);
	circle.setOutlineThickness(2.5f);
	circle.setOutlineColor(sf::Color::Red);

	//Loop through our bodies
	for (auto& body : bodies)
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
	bodies.push_back(&body); //Add the body to the vector of physics bodies
}

void PhysicsHandler::deletePhysicsBody(PhysicsBody & body)
{
	bodies.erase(std::remove_if(bodies.begin(), bodies.end(),
		[body](PhysicsBody* i) {return i && (*i == body); }));
}