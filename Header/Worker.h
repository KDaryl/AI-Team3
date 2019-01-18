#pragma once
#include "AI.h"

class Worker
{
public:
	Worker(Vector2f startPos, Grid* grid);

	void update(double dt);
	void draw(sf::RenderWindow& window);
	bool seek(Vector2f pos, double dt); //Returns a bool that indicates if we have reached the target or not

	//This will capture the worker, if it is captured by a sweeper, the worker will respawn at the sweepers location when it is destroyed
	void captureWorker(bool isPlayer, Vector2f& position);
	void freeWorker(); //Worke rstops following captor
	void setTexture(ResourceManager& resources);
	
	//getters
	Vector2f getPos() { return m_position; }
	int randInt(int min, int max);

	//Methods to change max speed and speed to follow capturer and to also reset if capturer is dead

	//getters
	bool& isCapturedByPlayer() { return m_playerCaptured; }
	bool& captured() { return m_captured; }
	CollisionCircle& rangeCollider() { return m_rangeCollider; }
private:
	enum WorkerState //Local to Worker
	{
		Wander,
		Captured
	};

	float m_followDistance = 12.5f; //Distance to stop following at

	WorkerState m_state;

	CollisionCircle m_rangeCollider;

	std::vector<Vector2f> m_seekPath;

	bool m_captured, m_playerCaptured; //Bools for captured by player or sweeper bot

	//Movement Variables
	sf::Vector2i m_gridPosition;
	Vector2f m_position, m_target, m_startPos;
	float m_angle, m_turnSpeed, m_moveSpeed, m_maxSpeed;
	Vector2f * capturePos; //The pos we follow when captured (seek to position of capturer)

	PhysicsBody m_body;

	Grid* m_gridPtr;

	sf::FloatRect m_gridRect;

	//Wander variables
	float m_wanderChange, m_currentWander;

	//Animation and Sprite variables
	thor::Animator<sf::Sprite, std::string> m_animator;
	sf::Sprite m_sprite;
	bool m_moving;
};
