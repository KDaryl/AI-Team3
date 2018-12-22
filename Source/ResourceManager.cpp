#include "ResourceManager.h"

ResourceManager::ResourceManager()
{
	//For good practice, we should use the name of the files as the id for the texture
	//For example, "Player" should be the id for "Player.png", this makes it easy to debug
	//And set textures

	//Load game related textures (player, ai, worker etc.)
	loadTexture("Player", "Ships/Player.png", true);

	//Load environment Textures (corridors, walls, backgrounds etc)
	loadTexture("Starfield BG", "Environment/Starfield BG.png", true);
	loadTexture("Straight Corridor", "Environment/Straight Corridor.png", true);
	loadTexture("Three Exit Room", "Environment/Three Exit Room.png", true);
	loadTexture("Top Left Corner Room", "Environment/Top Left Corner Room.png", true);
	loadTexture("Two Exit Room", "Environment/Two Exit Room.png", true);
}

void ResourceManager::loadTexture(std::string name, std::string fileName, bool smooth)
{
	sf::Texture texture;
	if (!texture.loadFromFile(m_filePath + fileName))
	{
		std::cout << "Error loading string" << std::endl;
	}
	texture.setSmooth(smooth);
	m_textureMap[name] = texture;
}

sf::Texture & ResourceManager::getTexture(std::string textureName)
{
	return m_textureMap[textureName];
}
