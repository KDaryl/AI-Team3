#include "InputHandler.h"
#include <iostream>

InputHandler::InputHandler()
{
	//Add buttons to our input handler, these are our defaults
	m_current["W"] = false;
	m_current["A"] = false;
	m_current["S"] = false;
	m_current["D"] = false;
	m_current["E"] = false;
	m_current["Q"] = false;
	m_current["Up"] = false;
	m_current["Left"] = false;
	m_current["Right"] = false;
	m_current["Down"] = false;
	m_current["Shift"] = false;
	m_current["Tab"] = false;
	m_current["LEFT_MOUSE"] = false;
	m_current["RIGHT_MOUSE"] = false;
	
	//Copy the values of current to previous
	m_previous = m_current;

	//Create collider for mouse
	m_mouseCollider.setSize(2, 2); //Make it small so we can easily see if the mouse is intersecting something
}

InputHandler::~InputHandler()
{
}

void InputHandler::update(sf::RenderWindow& win)
{
	m_previous = m_current; //Set our previous

	//Get mouse position relative to our game and not the desktop
	m_mousePos = sf::Mouse::getPosition(win);
		 
 	//Set mouse collider position
	m_mouseCollider.setPosition(m_mousePos);

	//Check if the keys are currently down (we can easily add key checks here)
	m_current["W"] = sf::Keyboard::isKeyPressed(sf::Keyboard::W);
	m_current["A"] = sf::Keyboard::isKeyPressed(sf::Keyboard::A);
	m_current["S"] = sf::Keyboard::isKeyPressed(sf::Keyboard::S);
	m_current["D"] = sf::Keyboard::isKeyPressed(sf::Keyboard::D);
	m_current["E"] = sf::Keyboard::isKeyPressed(sf::Keyboard::E);
	m_current["Q"] = sf::Keyboard::isKeyPressed(sf::Keyboard::Q);
	m_current["Up"] = sf::Keyboard::isKeyPressed(sf::Keyboard::Up);
	m_current["Left"] = sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
	m_current["Right"] = sf::Keyboard::isKeyPressed(sf::Keyboard::Right);
	m_current["Down"] = sf::Keyboard::isKeyPressed(sf::Keyboard::Down);
	m_current["Shift"] = sf::Keyboard::isKeyPressed(sf::Keyboard::LShift);
	m_current["Tab"] = sf::Keyboard::isKeyPressed(sf::Keyboard::Tab);
	m_current["LEFT_MOUSE"] = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left); //Get left mouse click 
	m_current["RIGHT_MOUSE"] = sf::Mouse::isButtonPressed(sf::Mouse::Button::Right); //Get right mouse click 
}

bool InputHandler::isButtonPressed(std::string button)
{
	return m_current[button] && !m_previous[button];
}

bool InputHandler::isButtonDown(std::string button)
{
	return m_current[button];
}
