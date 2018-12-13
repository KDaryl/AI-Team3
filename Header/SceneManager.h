#pragma once
#include "Scene.h"

class SceneManager
{
public:
	SceneManager();
	~SceneManager();

	void update(double dt);
	void draw(sf::RenderWindow& window);
	void addScene(std::string name, Scene* scene);

	void setCurrentScene(std::string sceneName);

private:
	Scene * m_current;
	std::map<std::string, Scene*> m_scenes;
};

