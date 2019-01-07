#include "CollisionCircle.h"

CollisionCircle::CollisionCircle() :
	position(0,0),
	r(0)
{
	setCircle();
}

CollisionCircle::CollisionCircle(float x, float y, float r) :
	position(x, y),
	r(r)
{
	setCircle();
}

CollisionCircle::~CollisionCircle()
{
}

void CollisionCircle::setCircle()
{
	circle.setFillColor(sf::Color::Transparent);
	circle.setOutlineThickness(2.5f);
	circle.setOutlineColor(sf::Color::Red);
	circle.setRadius(r);
	circle.setOrigin(circle.getGlobalBounds().width / 2, circle.getGlobalBounds().height / 2);
	circle.setPosition(position.x, position.y);
}

void CollisionCircle::setRadius(float _r)
{
	r = _r;
	setCircle();
}

void CollisionCircle::setSize(float x, float y, float _r)
{
	position = Vector2f(x, y);
	r = _r;
	setCircle();
}

void CollisionCircle::setPosition(float x, float y)
{
	position = Vector2f(x, y);
	circle.setPosition(position.x, position.y);
}

void CollisionCircle::setPosition(Vector2f pos)
{
	position = pos;
	circle.setPosition(position.x, position.y);
}

void CollisionCircle::draw(sf::RenderWindow & window)
{
	window.draw(circle);
}
