#include "Boundary.h"

Boundary::Boundary(float x, float y, float w, float h, Player& player) :
	m_playerPtr(&player),
	m_debugRect(sf::Vector2f(w, h)),
	m_collider(x, y, w, h)
{
	m_debugRect.setPosition(x, y);
	m_debugRect.setOutlineColor(sf::Color::Red);
	m_debugRect.setOutlineThickness(5);
	m_debugRect.setFillColor(sf::Color::Transparent);
}

Boundary::~Boundary()
{
}

void Boundary::update(double dt)
{

}

void Boundary::draw(sf::RenderWindow & win)
{
	win.draw(m_debugRect);
}
