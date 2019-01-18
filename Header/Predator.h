#pragma once
#include "EnemyBullet.h"

class Predator
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

	Predator(Vector2f& playerPos, Grid* grid, int idIn);
	~Predator();

	void update(double dt);
	void draw(sf::RenderWindow& window);
	bool seek(boolVecPair& p, double dt); //Returns a bool that indicates if we have reached the target or not
	void lookAtPlayer(double dt);
	void spawn(Vector2f spawnPos);


	//getters 
	float getMoveSpeed() { return m_moveSpeed; }
	void setTexture(ResourceManager& resources);
	void die(); //Predator dies

	Vector2f m_position;
	Vector2f m_velocity;
	int m_id = 0;

	////flock functions 
	void flock(std::vector<Predator>* p, int currentP);
	Vector2f separation(std::vector<Predator>* p, int currentP);
	Vector2f alignment(std::vector<Predator>* p, int currentP);
	Vector2f cohesion(std::vector<Predator>* p, int currentP);

	//flock variables
	float distanceBetweenMax = 300;
	float aliWeight = 1;
	float sepWeight = 1;
	float cohWeight = 1;
	int m_neighbours = 0;
	Vector2f m_flockVector;


private:
	bool m_alive;

	struct removeVisited
	{
		bool operator()(boolVecPair& pair)
		{
			return pair.visited;
		}
	};

	enum PredatorState //Local to Predator (could make this an AI state)
	{
		Seek,
		Attack
	};

	PredatorState m_state; //The state of the predator

	CollisionCircle m_rangeCollider;

	std::vector<boolVecPair> m_seekPath; //Positions to move to

	//Bullet variables
	std::vector<EnemyBullet> m_bullets;
	float m_fireRate, m_timeToFire;

	//Movement variables
	sf::Vector2i m_gridPosition;
	boolVecPair* m_target;
	boolVecPair m_playerPosPair;
	float m_angle, m_turnSpeed, m_moveSpeed, m_maxSpeed;
	Vector2f* m_playerPosPtr; //Ptr to the players position

	PhysicsBody m_body;

	Grid* m_gridPtr; //For path finding

	sf::FloatRect m_gridRect;

	sf::Sprite m_sprite;
};

