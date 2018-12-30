#pragma once
#include "Collisions.h"

//Our types of Bodies that we can have
enum Type
{
	Dynamic,
	Static
};
//Our types of shapes our bodies can ahve, Circles or Boxes
enum Shape
{
	Box,
	Circle
};

class PhysicsBody
{
public:
	//Constructor takes the type of body and the shape it will use
	PhysicsBody(Type type, Shape shape);
	~PhysicsBody();

	void update(float dt); //Update the physics body position and colliders
	void applyGravity(float gravity); //Applies gravity to the velocity of the physics body
	bool operator==(const PhysicsBody &b);//Equals operator

	//Setter methods
	//Set the velocity of the physics object
	void setVelocity(Vector2f vel);
	void setFriction(float f);
	void setRestitution(float resti);
	//Set the scalar for gravity
	void setGravityScalar(float val);
	//Sets up a box shape physics shape
	void setBoxParameters(Vector2f startPos, Vector2f size, bool useGravity);
	//Sets up a circle shape physics object
	void setCircleParameters(Vector2f startPos, float radius, bool useGravity);

	//Public variables
	Vector2f position, velocity, size;
	float radius, e, mass, gravityScalar, friction; //Radius, restitution, mass, gravity scalar & friction
	bool useGravity; //Boolean to use gravity or not
	Shape shape; //The shape of the physics Object (box, circle)
	Type type; //The type of the physics Object (dynamic, static)
	//The collider our physics object will use, this will be set using Shape
	CollisionBox * bCollider; 
	CollisionCircle * cCollider;
};