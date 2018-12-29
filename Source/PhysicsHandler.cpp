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

bool PhysicsHandler::compareBodies(PhysicsBody& a, PhysicsBody& b)
{
	//If the two physics objects have the same shape, position, velocity, mass and the
	//same type of physics object (static, dynamic) then it is the same object
	//We could probably just check if the pointers reference the same memory address?
	if (a.shape == b.shape && a.velocity == b.velocity &&
		a.position == b.position && a.mass == b.mass && a.type == b.type)
		return true;

	return false;
}
