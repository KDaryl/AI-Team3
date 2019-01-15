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

void SceneManager::draw(sf::RenderWindow & window, float a)
{
	if (nullptr != m_current)
	{
		m_current->draw(window, a);
	}
}

void SceneManager::handleInput(InputHandler & input)
{
	if (nullptr != m_current)
	{
		m_current->handleInput(input);
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

void SceneManager::setTextures()
{
	//Loop through all our sevenes and pass our loaded resources object
	for (auto& p : m_scenes)
	{
		p.second->setTexture(m_resources);
	}
}
