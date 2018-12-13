#include "..\Header\SceneManager.h"

SceneManager::SceneManager()
{
}

SceneManager::~SceneManager()
{
}

void SceneManager::update(double dt)
{
	if (nullptr != m_current)
	{
		m_current->update(dt);
	}
}

void SceneManager::draw(sf::RenderWindow & window)
{
	if (nullptr != m_current)
	{
		m_current->draw(window);
	}
}

void SceneManager::addScene(std::string name, Scene* scene)
{
	m_scenes[name] = scene;
}

void SceneManager::setCurrentScene(std::string sceneName)
{
	m_current = m_scenes[sceneName]; //Set current scene
}
