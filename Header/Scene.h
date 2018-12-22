#pragma once
#include "SFML/Graphics.hpp"
#include "InputHandler.h"
#include "ResourceManager.h"

/// <summary>
/// This class will be inherited by any other scene classes, like Main Menu, game, options etc.
/// </summary>
class Scene
{
public:
	Scene() {};
	~Scene() {};

	virtual void update(double dt) = 0;
	virtual void draw(sf::RenderWindow& window) = 0;
	virtual void handleInput(InputHandler& input) = 0;
	virtual void setTexture(ResourceManager& resources) = 0;
};

