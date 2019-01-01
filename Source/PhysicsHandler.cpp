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
	//Loop through our bodies
	for (auto& body : physics::world->bodies)
	{
		//Draw the body
		body->draw(window);
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