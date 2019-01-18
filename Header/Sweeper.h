#pragma once
#include "AI.h"
#include "Worker.h"
#include <algorithm>
/**
* Sweeper bot AI 
* the function of this AI is to wander around the ship capturing workers and fleeing from the player when he comes too close 
*/
class Sweeper : public AI
{
public:
	struct boolVecPair
	{
		boolVecPair() {}
		boolVecPair(Vector2f p, bool b) :
			pos(p),
			visited(b)
		{
		}
		Vector2f pos;
		bool visited = false;
	};

	Sweeper(Vector2f& playerPos, Grid* grid, Vector2f pos, std::vector<Worker>* workers);
	~Sweeper();
	void update(float dt);
	void render(sf::RenderWindow& window);
	bool seek(boolVecPair& p, double dt);
	bool fleePlayer(boolVecPair& p, double dt);
	void die(); 

	//getters 
	float getMoveSpeed() { return m_moveSpeed; }
	//setters
	void setTexture(ResourceManager& resources);

	Vector2f m_position;
	Vector2f m_velocity;
	std::vector<Worker>* m_seekWorkers;
	std::vector<Worker*> m_capturedWorkers;

private:
	bool m_alive;

	struct removeVisited
	{
		bool operator()(boolVecPair& pair)
		{
			return pair.visited;
		}
	};

	enum SweeperState //Local to Predator (could make this an AI state)
	{
		Seek,
		Flee
	};

	SweeperState m_state;
	CollisionCircle m_rangeCollider;
	std::vector<boolVecPair> m_seekPath;
	int m_currentWorker;

	//Movement variables
	sf::Vector2i m_gridPosition;
	boolVecPair* m_target;
	boolVecPair m_playerPosPair;
	float m_angle, m_turnSpeed, m_moveSpeed, m_maxSpeed;
	Vector2f* m_playerPosPtr; 
	PhysicsBody m_body;
	Grid* m_gridPtr; 
	sf::FloatRect m_gridRect;
	sf::Sprite m_sprite;
};
