#include "PhysicsHandler.h"
#include <algorithm>

PhysicsHandler::PhysicsHandler()
{
}

PhysicsHandler::~PhysicsHandler()
{
}

void PhysicsHandler::update(float dt)
{
	physics::dt = dt; //Set DT

	//Loop through our bodies and update them
	for (auto& body : physics::world->bodies)
	{
		//Only update the body if it is not a static
		//If our body uses gravity, add gravity to our bodies velocity
		if (body->useGravity)
			body->applyGravity(physics::world->gravity * dt);

		//Update the body
		body->update(dt);
	}
}

void PhysicsHandler::checkCollision()
{
	//Collision Checking
	for (auto& body : physics::world->bodies)
	{
		//If the body is not a staic, dont check for collisions
		if (body->type != Type::Static)
		{
			for (auto& other : physics::world->bodies)
			{
				if (body != other) //If the bodies are not the same, check for collision
				{

					//Create the manifold here
					Manifold m = Manifold(body, other);

					//Flip both bodies Y velocity
					body->velocity.y = -body->velocity.y;
					other->velocity.y = -other->velocity.y;

					//If The body we want to check is colliding off something, check what shape it is
					if (body->shape == Shape::Box)
					{
						//If the other body is a box, and they collide
						if (other->shape == Shape::Box && AABBvsAABB(m))
						{
							//If the collision boxes have collided, resolve collision
							resolveCollision(m);
						}
						//Else check AABBvsCircle, needs to be implemented
						if (other->shape == Shape::Circle && AABBvsCircle(m))
						{
							//If the collision boxes have collided, resolve collision
							resolveCollision(m);
						}

					}
					else
					{
						//Check circle vs circle collision
						if (other->shape == Shape::Circle && CirclevsCircle(m))
						{
							//If the collision circles have collided, resolve collision
							resolveCollision(m);
						}
						//Create the manifold here
						Manifold m = Manifold(other, body);

						//Else check circle vs AABB collision
						if (other->shape == Shape::Box && AABBvsCircle(m))
						{
							resolveCollision(m);
						}
					}
				}
			}
		}
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

void PhysicsHandler::resolveCollision(Manifold& m)
{
	std::cout << "Resolving Collision" << std::endl;

	//Calculate relative velocity
	Vector2f rv = m.B->velocity - m.A->velocity;
	auto normal = rv.normalise(); //Get the normal

	//Calculate relative velocity in terms of the normal direction
	float velAlongNormal = rv.dotProduct(normal);;

	//Do not resolve if velocities are separating
	if (velAlongNormal < 0)
		return;

	// Calculate restitution
	float e = min(m.A->e, m.B->e);

	// Calculate impulse scalar
	float j = -(1 + e) * velAlongNormal;
	j /= m.A->inv_mass + m.B->inv_mass;

	//Apply impulse
	Vector2f impulse = normal * j;

	//Since we want smalle robjects to bounce more off bigger objects
	//We need to get a ratio of the impulse to apply to each object
	float mass_sum = m.A->mass + m.B->mass;
	float ratio = m.A->mass / mass_sum;
	m.A->velocity -= impulse * ratio;
	ratio = m.B->mass / mass_sum;
	m.B->velocity += impulse * ratio;

	//Correct the position of the objects (reduce jitter, and fix sinking objects)
	positionalCorrection(m);
}

void PhysicsHandler::positionalCorrection(Manifold& m)
{
	float percent = 0.2f; //Usually 20 to 80 percent
	float slop = 0.01f; //Usually 0.01 to 0.1
	Vector2f correction = m.normal * (max(m.penetration - slop, 0.0f) / (m.A->inv_mass + m.B->inv_mass) * percent);
	m.A->position -= correction * m.A->inv_mass * (physics::dt * 3);
	m.B->position += correction * m.B->inv_mass * (physics::dt * 3);

	//Flip both bodies Y velocity
	m.A->velocity.y = -m.A->velocity.y;
	m.B->velocity.y = -m.B->velocity.y;
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

bool PhysicsHandler::AABBvsAABB(Manifold& m)
{
	CollisionBox* a = m.A->bCollider;
	CollisionBox* b = m.B->bCollider;

	//Vector from A to B
	Vector2f n = b->position - a->position;

	// Calculate half extents along x axis for each object
	float a_extent = (a->max.x - a->min.x) / 2;
	float b_extent = (b->max.x - b->min.x) / 2;

	// Calculate overlap on x axis
	float x_overlap = a_extent + b_extent - abs(n.x);

	//SAT test on x axis
	if (x_overlap > 0)
	{
		// Calculate half extents along y axis for each object
		a_extent = (a->max.y - a->min.y) / 2;
		b_extent = (b->max.y - b->min.y) / 2;

		//Calculate overlap on Y axis
		float y_overlap = a_extent + b_extent - abs(n.y);

		//SAT test on Y axis
		if (y_overlap > 0)
		{
			// Find out which axis is axis of least penetration
			if (x_overlap < y_overlap)
			{
				// Point towards B knowing that n points from A to B
				if (n.x < 0)
					m.normal = Vector2f(-1, 0);
				else
					m.normal = Vector2f(1, 0);
				m.penetration = x_overlap;
				return true;
			}
			else
			{
				// Point toward B knowing that n points from A to B
				if (n.y < 0)
					m.normal = Vector2f(0, -1);
				else
					m.normal = Vector2f(0, 1);
				m.penetration = y_overlap;
				return true;
			}
		}
	}
	
	return false;
}

bool PhysicsHandler::CirclevsCircle(Manifold& m)
{
	CollisionCircle* a = m.A->cCollider;
	CollisionCircle* b = m.B->cCollider;

	//Vector from A to B
	Vector2f n = b->position - a->position;

	float r = a->r + b->r;
	r *= r;

	//If the circles have not collided, return
	if (n.sqrdMagnitude() > r)
		return false;

	//Circles have collided, generate manifold
	float d = n.magnitude();

	//If the distance between circles is not 0
	if (d != 0)
	{
		//Penetration is difference between radius and distance
		m.penetration = sqrt(r) - d;
		//Normal is the normal of N divided by D the distance between the circles
		m.normal = n.normalise() / d;
		return true;
	}
	//Else the circles are at the same position
	else
	{
		//Choose constant valeus for the manifold if they are at teh same position
		m.penetration = m.A->radius;
		m.normal = Vector2f(1, 0);
		return true;
	}
}
bool PhysicsHandler::AABBvsCircle(Manifold& m)
{
	CollisionBox* a = m.A->bCollider;
	CollisionCircle* b = m.B->cCollider;

	//Vector from A to B
	Vector2f n = b->position - a->position;

	// Closest point on A to center of B
	Vector2f closest = n;

	// Calculate half extents along x axis for each object
	float x_extent = (a->max.x - a->min.x) / 2;
	float y_extent = (a->max.y - a->min.y) / 2;

	// Clamp point to edges of the AABB
	closest.x = clamp(-x_extent, x_extent, closest.x);
	closest.y = clamp(-y_extent, y_extent, closest.y);

	bool inside = false;

	// Circle is inside the AABB, so we need to clamp the circle's center
	 // to the closest edge
	if (n == closest)
	{
		inside = true;

		// Find closest axis
		if (abs(n.x) > abs(n.y))
		{
			// Clamp to closest extent
			if (closest.x > 0)
				closest.x = x_extent;
			else
				closest.x = -x_extent;
		}
		// y axis is shorter
		else
		{
			// Clamp to closest extent
			if (closest.y > 0)
				closest.y = y_extent;
			else
				closest.y = -y_extent;
		}
	}

	Vector2f normal = n - closest;
	float d = normal.sqrdMagnitude();
	float r = b->r;

	// Early out of the radius is shorter than distance to closest point and
	// Circle not inside the AABB
	if (d > r * r && !inside)
		return false;

	// Avoided sqrt until we needed
	d = sqrt(d);

	// Collision normal needs to be flipped to point outside if circle was
	// inside the AABB
	if (inside)
	{
		m.normal.x = -n.x;
		m.normal.y = -n.y;
		m.penetration = r - d;
	}
	else
	{
		m.normal = n;
		m.penetration = r - d;
	}

	return true;
}


float PhysicsHandler::clamp(float minNum, float maxNum, float num)
{
	return min(maxNum, max(num, minNum));
}
float PhysicsHandler::min(float a, float b)
{
	return a < b ? a : b;
}

float PhysicsHandler::max(float a, float b)
{
	return a > b ? a : b;
}
