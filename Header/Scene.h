#pragma once
#include "SFML/Graphics.hpp"
#include "InputHandler.h"
#include "ResourceManager.h"

/**
* Description: Base scene class, to be inherited by other other scene classes
*/
class Scene
{
public:
	Scene() {};
	~Scene() {};

	virtual void update(double dt) = 0;
	virtual void draw(sf::RenderWindow& window, float a) = 0;
	virtual void handleInput(InputHandler& input) = 0;
	virtual void setTexture(ResourceManager& resources) = 0;
};

