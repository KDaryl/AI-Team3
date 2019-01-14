#pragma once
#include "AI.h"

class Wander: public AI
{
public:
	Wander(Vector2f position, float maxSpeed);
	~Wander();
	Vector2f calculateWander();
	void update(Vector2f t);
	void render(sf::RenderWindow& window);

private:
	Vector2f m_circleCenter;
	Vector2f m_wanderForce;
	Vector2f m_displacement;

	float m_wanderAngle;
	float m_randomNum;
	const float CIRCLE_DISTANCE = 100.0f;
	const float CIRCLE_RADIUS = 25.0f;
	const float ANGLE_CHANGE = 1.0f;
};

