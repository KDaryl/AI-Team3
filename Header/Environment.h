#pragma once
#include "SFML/Graphics.hpp"
#include "ResourceManager.h"
#include "Vector2f.h"

class Environment
{
public:
	Environment(std::string _tag, float x, float y);
	~Environment();

	void draw(sf::RenderWindow& win);
	void setTexture(ResourceManager& resources, std::string textureName);
	
	std::string tag;

	//Setters
	void setRotation(float angle);
	void setScale(float x, float y);

	//Getters
	sf::FloatRect collider() { return m_collider; }
private:
	Vector2f m_position;
	sf::FloatRect m_collider;
	sf::Sprite m_sprite;
};

