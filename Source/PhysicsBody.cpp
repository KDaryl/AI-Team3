#include "PhysicsBody.h"

PhysicsBody::PhysicsBody(Type _type, Shape _shape, void* data) :
	type(_type),
	shape(_shape),
	radius(0), 
	e(1),
	mass(1),
	friction(1),
	gravityScalar(1),
	objectData(data)
{
	//Set up our collider usign our shape provided
	if (shape == Shape::Box)
		bCollider = new CollisionBox();
	else
		cCollider = new CollisionCircle();

	//Setting our inverse mass
	if (mass == 0)
		inv_mass = 0;
	else
		inv_mass = 1 / mass;
}

PhysicsBody::~PhysicsBody()
{
}

void PhysicsBody::update(float dt)
{
	velocity *= friction; //Multiply velocity by friction
	position += velocity; //Add velocity to our position

	switch (shape)
	{
		//Collider is centered on position
		case Shape::Box:
			bCollider->setPosition(position);
		break;
		case Shape::Circle:
			cCollider->setPosition(position);
		break;
	}
}

void PhysicsBody::draw(sf::RenderWindow & window)
{
	switch (shape)
	{
	case Shape::Box:
		bCollider->draw(window);
		break;
	case Shape::Circle:
		cCollider->draw(window);
		break;
	}
}

void PhysicsBody::applyGravity(float gravity)
{
	velocity.y += (gravity * gravityScalar); //Add gravity to our velocity
}

bool PhysicsBody::operator==(const PhysicsBody & b)
{
	PhysicsBody a = b; //Assign b to a so we can convert from const to non const for comparisons
	if (a.shape == shape && b.type == type && a.position == position &&
		a.velocity == velocity && b.mass == mass)
		return true;

	return false;
}

void PhysicsBody::setVelocity(Vector2f vel)
{
	velocity = vel;
}

void PhysicsBody::setFriction(float f)
{
	friction = f;
}

void PhysicsBody::setRestitution(float resti)
{
	e = resti;
}

void PhysicsBody::setGravityScalar(float val)
{
	gravityScalar = val;
}

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

void PhysicsBody::setCircleParameters(Vector2f startPos, float _radius, float _mass, bool _useGravity)
{
	position = startPos;
	radius = _radius;
	cCollider->setSize(startPos.x, startPos.y, radius);//Set radius and position of the circle collider
	useGravity = _useGravity;
	mass = _mass;
	//Setting our inverse mass
	if (mass == 0)
		inv_mass = 0;
	else
		inv_mass = 1 / mass;
}
