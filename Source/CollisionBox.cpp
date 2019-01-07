#include "CollisionBox.h"

CollisionBox::CollisionBox() :
	position(0, 0),
	w(0),
	h(0),
	min(0, 0),
	max(0, 0)
{
	setBox();
}

CollisionBox::CollisionBox(float x, float y, float w, float h) :
	position(x, y),
	w(w),
	h(h),
	min(position.x - w / 2, position.y - h / 2),
	max(position.x + w, position.y + h)
{
	setBox();
}

CollisionBox::~CollisionBox()
{
}

void CollisionBox::setBox()
{
	rect.setFillColor(sf::Color::Transparent);
	rect.setOutlineThickness(2.5f);
	rect.setOutlineColor(sf::Color::Red);
	rect.setSize(sf::Vector2f(w, h));
	rect.setOrigin(rect.getGlobalBounds().width / 2, rect.getGlobalBounds().height / 2);
	rect.setPosition(position.x, position.y);
}

void CollisionBox::setSize(float _w, float _h)
{
	w = _w;
	h = _h;
	setBox();
}

void CollisionBox::setSize(float x, float y, float _w, float _h)
{
	position.x = x;
	position.y = y;
	w = _w;
	h = _h;
	setBox();
}

void CollisionBox::setPosition(float x, float y)
{
	position = Vector2f(x, y); //Set the position
	rect.setPosition(position.x, position.y); //Set rect position
	min = Vector2f(rect.getGlobalBounds().left, rect.getGlobalBounds().top); //Set the min position
	max = Vector2f(min.x + w, min.y + h); //Set the max
}

void CollisionBox::setPosition(Vector2f pos)
{
	position = pos;
	rect.setPosition(position.x, position.y); //Set rect position
	min = Vector2f(rect.getGlobalBounds().left, rect.getGlobalBounds().top); //Set the min position
	max = Vector2f(min.x + w, min.y + h); //Set the max
}

void CollisionBox::draw(sf::RenderWindow & window)
{
	window.draw(rect); //Draw the rectangle for the collider
}
