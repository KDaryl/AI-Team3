#pragma once
#include "SFML/Graphics.hpp"
#include "CollisionBox.h"
#include "Vector2f.h"
#include <string>
#include <map>

typedef std::map<std::string, bool> Input; //Use input as an alias for our map of buttons

/**
* Description: This is where we will check for input handling
*/

class InputHandler
{
public:
	InputHandler();
	~InputHandler();

	void update(sf::RenderWindow& win);

	//Checks if a button has been pressed
	bool isButtonPressed(std::string button);
	//Checks if a button is being held down
	bool isButtonDown(std::string button);

	//Getters
	int getMouseX() { return m_mousePos.x; }
	int getMouseY() { return m_mousePos.y; }
	Vector2f getMousePos() { return m_mousePos; }
	CollisionBox& getMouseCollider() { return m_mouseCollider; }
private:
	Vector2f m_mousePos;
	Input m_current;
	Input m_previous;
	CollisionBox m_mouseCollider;
};

