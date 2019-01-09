#pragma once
#include "PhysicsBody.h"

class PhysicsHandler
{
public:
	//Manifold object for collision resolution
	struct Manifold
	{
		Manifold(PhysicsBody* _a, PhysicsBody* _b) :
			A(_a),
			B(_b)
		{}
		PhysicsBody* A;
		PhysicsBody* B;
		float penetration;
		Vector2f normal;
	};

	PhysicsHandler();
	~PhysicsHandler();

	static void update(float dt);
	static void checkCollision();
	static void draw(sf::RenderWindow& window);
	static void resolveCollision(Manifold& m);
	static void positionalCorrection(Manifold& m);
	

	static void addPhysicsBody(PhysicsBody& body);
	static void deletePhysicsBody(PhysicsBody& body);

	static bool AABBvsAABB(Manifold& m);
	static bool CirclevsCircle(Manifold& m);
	static bool AABBvsCircle(Manifold& m);

private:
	std::vector<PhysicsBody*> bodies; //The vector of physics bodies, these are pointers
	const float gravity = 9.8; //The value of gravity
	static float min(float a, float b); //Returns the minimum value fo the two
	static float max(float a, float b); //Returns the maximum value fo the two
	static float clamp(float minNum, float maxNum, float num);
};

//NameSpace physics, we declare this here as we can then call the physics world very easily and also add delete bodies from anywhere necessary
namespace physics
{
	static float dt;
	static PhysicsHandler* world = new PhysicsHandler(); //Our world, from here we can add, delete, update and draw bodies
}