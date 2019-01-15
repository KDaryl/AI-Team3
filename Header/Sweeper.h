#pragma once
#include "AI.h"

class Sweeper : public AI
{
public:
	Sweeper(Player* p, float maxSpeed, Vector2f pos);
	~Sweeper();
	void update(float dt);
	void render(sf::RenderWindow& window);

private:
	bool m_seeWorker = false;
};

