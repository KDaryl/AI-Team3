#pragma once
#include "Predator.h"
#include "NestMissile.h"

/**
* Description: The nest spawns predators and shoots players that are nearby
*/
class Nest
{
public:
	Nest(Player& player, Grid& grid, Vector2f pos, std::vector<Predator>& preds);
	~Nest();

	void update(double dt, std::vector<Nest>& otherNests);
	void draw(sf::RenderWindow& window);
	void die();
	void setTexture(ResourceManager& resources);

	void addDecHealth(int val);

	bool isBuilding() { return m_isBuilding; }
	sf::FloatRect& collider() { return m_collider; }
private:
	const float m_buildTime = 30; //Tghe time it takes to build something
	float m_currentBuildTime; //The time till we build something
	const float m_fireRate = 3; //3 Seconds till we can fire again
	float m_timeTillFire; //The time till we fire
	int m_health = 100;
	bool m_alive;
	bool m_isBuilding;
	sf::Sprite m_sprite;
	Vector2f m_position;
	Player* m_playerPtr;
	sf::FloatRect m_collider;
	std::vector<Predator>* m_predators; //The Vector of predators
	Predator* m_predToSpawn; //The predator we wan to spawn
	NestMissile m_missile;
	PhysicsBody m_body;// The body of the nest, this will be a sensor
};

