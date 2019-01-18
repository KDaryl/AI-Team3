#include "PhysicsBody.h"

/**
* Description: PhysicBody contructor
*/
PhysicsBody::PhysicsBody(Type _type, Shape _shape, void* data) :
	type(_type),
	shape(_shape),
	radius(0), 
	e(1),
	mass(1),
	friction(1),
	gravityScalar(1),
	mask(0), //The layer im on
	objectData(data)
{
	//Set up our collider usign our shape provided
	if (shape == Shape::Box)
		bCollider = new CollisionBox();
	else
	{
		bCollider = new CollisionBox();
		cCollider = new CollisionCircle();
	}


	//Setting our inverse mass
	if (mass == 0)
		inv_mass = 0;
	else
		inv_mass = 1 / mass;

	if(type == Type::Static)
		e = 0.4f;
}

PhysicsBody::~PhysicsBody()
{
}

/**
* Description: Updates the physics bodies position
*/
void PhysicsBody::update(float dt)
{
	collisionResolved = false;

	if (velocity.magnitude() < 0.01)
		velocity.zeroVector();

	position += velocity * dt; //Add velocity to our position

	switch (shape)
	{
		//Collider is centered on position
		case Shape::Box:
			bCollider->setPosition(position);
		break;
		case Shape::Circle:
			cCollider->setPosition(position);
			bCollider->setPosition(position);
		break;
	}

	velocity *= friction;
}

/**
* Description: Draws the physics bodies colliders
*/
void PhysicsBody::draw(sf::RenderWindow & window)
{
	switch (shape)
	{
	case Shape::Box:
		bCollider->draw(window);
		break;
	case Shape::Circle:
		cCollider->draw(window);
		bCollider->draw(window);
		break;
	}
}
/**
* Description: Applies gravity to the physics body
*/
void PhysicsBody::applyGravity(float gravity)
{
	velocity.y += (gravity * gravityScalar); //Add gravity to our velocity
}

/**
* Description: Equality operator for comparisons
*/
bool PhysicsBody::operator==(const PhysicsBody & b)
{
	PhysicsBody a = b; //Assign b to a so we can convert from const to non const for comparisons
	if (a.shape == shape && b.type == type && a.position == position &&
		a.velocity == velocity && b.mass == mass)
		return true;

	return false;
}

/**
* Description: Sets the bodies velocity
*/
void PhysicsBody::setVelocity(Vector2f vel)
{
	velocity = vel;
}

/**
* Description: Sets the friction of the body
*/
void PhysicsBody::setFriction(float f)
{
	friction = f;
}

/**
* Description: Sets the restitution of the body (how much of its velocity is lost on impact)
*/
void PhysicsBody::setRestitution(float resti)
{
	e = resti;
}

/**
* Description: Sets the initial rotation of the body
*/
void PhysicsBody::setInitialRotation(float angle)
{
	if (shape == Shape::Box)
	{
		bCollider->rotate(angle);
	}
}

/**
* Description: Adds a force to the velocity
*/
void PhysicsBody::addForce(Vector2f force)
{
	velocity += force * mass;
}

/**
* Description: Sets the gravity scalar
*/
void PhysicsBody::setGravityScalar(float val)
{
	gravityScalar = val;
}

/**
* Description: Sets the box collider properties
*/
void PhysicsBody::setBoxParameters(Vector2f startPos, Vector2f _size, float _mass, bool _useGravity)
{
	position = startPos;
	size = _size;
	bCollider->setSize(startPos.x, startPos.y, size.x, size.y);//Set size and position of the box collider
	useGravity = _useGravity;
	mass = _mass;
	//Setting our inverse mass
	if (mass == 0)
		inv_mass = 0;
	else
		inv_mass = 1 / mass;
}

/**
* Description: Sets the Circle collider properties
*/
void PhysicsBody::setCircleParameters(Vector2f startPos, float _radius, float _mass, bool _useGravity)
{
	position = startPos;
	radius = _radius;
	cCollider->setSize(startPos.x, startPos.y, radius);//Set radius and position of the circle collider
	bCollider->setSize(startPos.x, startPos.y, radius * 2, radius * 2);
	useGravity = _useGravity;
	mass = _mass;
	//Setting our inverse mass
	if (mass == 0)
		inv_mass = 0;
	else
		inv_mass = 1 / mass;
}
