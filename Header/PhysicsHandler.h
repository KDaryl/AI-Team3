#pragma once
#include "PhysicsBody.h"

class PhysicsHandler
{
public:
	PhysicsHandler();
	~PhysicsHandler();

	void update(float dt);
	void draw(sf::RenderWindow& window);

	void addPhysicsBody(PhysicsBody& body);
	void deletePhysicsBody(PhysicsBody& body);

private:
	//OOur comparer for physics bodies, this will allow us to remove bodies from the world
	bool compareBodies(PhysicsBody& a, PhysicsBody& b);

	std::vector<PhysicsBody*> bodies; //The vector of physics bodies, these are pointers
	const float gravity = 9.8; //The value of gravity
};

//NameSpace physics, we declare this here as we can then call the physics world very easily and also add delete bodies from anywhere necessary
namespace physics
{
	PhysicsHandler* world;
}
