#include "ResourceManager.h"

ResourceManager::ResourceManager()
{
	//Load all of our textures
}

void ResourceManager::loadTexture(std::string name, std::string fileName)
{
	sf::Texture texture;
	if (!texture.loadFromFile(m_filePath + fileName))
	{
		std::cout << "Error loading string" << std::endl;
	}
	m_textureMap[name] = texture;
}

sf::Texture & ResourceManager::getTexture(std::string textureName)
{
	return m_textureMap[textureName];
}
