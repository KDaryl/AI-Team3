#include "PhysicsBody.h"

PhysicsBody::PhysicsBody(Type _type, Shape _shape) :
	type(_type),
	shape(_shape),
	radius(0), 
	e(0),
	mass(0),
	friction(1)
{
	//Set up our collider usign our shape provided
	if (shape == Shape::Box)
		bCollider = new CollisionBox();
	else
		cCollider = new CollisionCircle();

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
		case Shape::Box:
			bCollider->setPosition(position);
		break;
		case Shape::Circle:
			cCollider->setPosition(position);
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

void PhysicsBody::setBoxParameters(Vector2f startPos, Vector2f size, bool _useGravity)
{
	bCollider->setSize(startPos.x, startPos.y, size.x, size.y);//Set size and position of the box collider
	useGravity = _useGravity;
}

void PhysicsBody::setCircleParameters(Vector2f startPos, float radius, bool _useGravity)
{
	cCollider->setSize(startPos.x, startPos.y, radius);//Set radius and position of the circle collider
	useGravity = _useGravity;
}
