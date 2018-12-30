#pragma once
#include "PhysicsBody.h"

class PhysicsHandler
{
public:
	PhysicsHandler();
	~PhysicsHandler();

	static void update(float dt);
	static void draw(sf::RenderWindow& window);

	static void addPhysicsBody(PhysicsBody& body);
	static void deletePhysicsBody(PhysicsBody& body);

private:
	std::vector<PhysicsBody*> bodies; //The vector of physics bodies, these are pointers
	const float gravity = 9.8; //The value of gravity
};

//NameSpace physics, we declare this here as we can then call the physics world very easily and also add delete bodies from anywhere necessary
namespace physics
{
	static PhysicsHandler* world = new PhysicsHandler(); //Our world, from here we can add, delete, update and draw bodies
}