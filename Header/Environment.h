#pragma once
#include "SFML/Graphics.hpp"
#include "ResourceManager.h"
#include "Vector2f.h"

//For reference, dont uncomment
//enum Tag
//{
//	"ROE" : Room One Exit
//	"CTE" : Corner Two Exits
//	"BRFE" : Big Room Four Exits
//	"2ER" : Two Exit room
//	"SC" : Straight Corridor
//	"CFJ" : Corridor 4 Junction
//	"3ER" : Three exit room
//	"TLCR" : Top Left Corner room
//}

class Environment
{
public:
	Environment(float x, float y, std::string _tag);
	~Environment();

	void draw(sf::RenderWindow& win);
	void setTexture(ResourceManager& resources, std::string textureName);

	//Setters
	void setRotation(float angle);
	void setScale(float x, float y);

	std::string tag;
	Vector2f m_position;
	sf::Sprite m_sprite;
	float angle;

	//Getters
	sf::FloatRect collider() { return m_collider; }
private:
	sf::FloatRect m_collider;
};

