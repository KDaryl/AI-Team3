#pragma once
#include "AI.h"

class NestMissile
{
public:
	NestMissile(Vector2f& playerPos, Grid* grid);

	void update(double dt);

	void draw(sf::RenderWindow& window);

	void spawn(Vector2f position, float angle);

	void setTexture(ResourceManager& resources);

	void hasCollided();
	const float ttl = 5; //The timeToLive is 5 seconds
	const int damage = 50;
	float timeAlive;
	bool alive;
	bool collided;
	bool dead;
	Vector2f m_position, m_velocity;
private:
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
	struct removeVisited
	{
		bool operator()(boolVecPair& pair)
		{
			return pair.visited;
		}
	};
	std::vector<boolVecPair> m_seekPath; //Positions to move to

	bool seek(boolVecPair& p, double dt);

	boolVecPair* m_target;
	boolVecPair m_playerPosPair;
	Vector2f* m_playerPosPtr; //Ptr to the players position
	float m_angle, m_turnSpeed, m_moveSpeed, m_maxSpeed; //The angle and speed of the bullet

	PhysicsBody m_body;

	Grid* m_gridPtr; //For path finding

	//Sprite and animation variables
	thor::Animator<sf::Sprite, std::string> m_animator; //Animator
	sf::Sprite m_sprite;
};