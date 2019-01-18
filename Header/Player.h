#pragma once
#include "PlayerBullet.h"

/**
* Class : Player
* Description: The player is controlled by the user using the Arrow keys or W,A,S and D keys.
* the player can shoot using the spacebar.
*/
class Player
{
public:
	Player(float x, float y);
	~Player() {}

	void update(double dt);
	void draw(sf::RenderWindow& win, float a);
	void handleInput(InputHandler& input);
	void setTexture(ResourceManager& resources);
	void setAudioRef(ResourceManager& resources);
	void addDelHealth(int val); //Decrements or adds a value to the players health
	void die(); //Kill the player

	sf::Event e;
	//Public variables
	Vector2f m_position, m_prevPos;
	Vector2f m_velocity;
	CollisionCircle m_rangeCollider;
	sf::Sprite m_sprite;
	PhysicsBody m_physicsBody;
	ResourceManager* ourResources;
	int health;
	int workersCollected;
	int maxHealth;
private:
	bool alive;
	sf::Sprite m_deathSprite;
	//Shooting Variables
	//Player bullets, we pool them to avoid nunecessary creation/deletion
	std::vector<PlayerBullet> m_bullets;
	float m_fireRate, m_timeToFire;

	//Private methods
	void setupAnimations();

	Vector2f m_turnVector;
	float m_moveSpeed, m_turnSpeed, m_friction, m_angle, m_dt, m_maxSpeed;

	sf::FloatRect m_gridRect;

	//Sprite and animation variables
	thor::Animator<sf::Sprite, std::string> m_animator; //Animator

	bool m_isMoving, m_turningLeft, m_turningRight;
};
