#pragma once
#include "SFML/Graphics.hpp"
#include "ResourceManager.h"
#include "Vector2f.h"

//For reference, dont uncomment
//enum Tag
//{
//	"CTE" : Corner Two Exits
//	"2E" : Two Exit room
//	"SC" : Straight Corridor
//	"SCF" : Straight Corridor Flipped
//	"3ER" : Three exit room
//	"3ERF-90" : Three exit room, Flipped, rotated -90 
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
	float angle;

	//Getters
	sf::FloatRect collider() { return m_collider; }
private:
	sf::FloatRect m_collider;
	sf::Sprite m_sprite;
};

