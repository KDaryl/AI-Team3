#pragma once
#include "ResourceManager.h"
#include "GameScene.h"

class SceneManager
{
public:
	SceneManager();
	~SceneManager();

	void update(double dt);
	void draw(sf::RenderWindow& window);
	void handleInput(InputHandler& input);
	void addScene(std::string name, Scene* scene);
	void setCurrentScene(std::string sceneName);
	void setTextures();

private:
	Scene * m_current;
	std::map<std::string, Scene*> m_scenes; //The map of scenes
	ResourceManager m_resources; //Our resources
};

