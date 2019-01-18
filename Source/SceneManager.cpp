#include "..\Header\SceneManager.h"

SceneManager::SceneManager()
{
}

SceneManager::~SceneManager()
{
}

/**
* Description: Updates current scene
*/
void SceneManager::update(double dt)
{
	if (nullptr != m_current)
	{
		m_current->update(dt);
	}
}

/**
* Description: draws the current scene
*/
void SceneManager::draw(sf::RenderWindow & window, float a)
{
	if (nullptr != m_current)
	{
		m_current->draw(window, a);
	}
}

/**
* Description: Here we call handle input for the scene and pass over the input handler
*/
void SceneManager::handleInput(InputHandler & input)
{
	if (nullptr != m_current)
	{
		m_current->handleInput(input);
	}
}

/**
* Description: Adds a scene to the scene manager
*/
void SceneManager::addScene(std::string name, Scene* scene)
{
	m_scenes[name] = scene;
}

/**
* Description: Sets the current scene
*/
void SceneManager::setCurrentScene(std::string sceneName)
{
	m_current = m_scenes[sceneName]; //Set current scene
}

/**
* Description: Passes over the resource manage rto every scene
*/
void SceneManager::setTextures()
{
	//Loop through all our sevenes and pass our loaded resources object
	for (auto& p : m_scenes)
	{
		p.second->setTexture(m_resources);
	}
}
