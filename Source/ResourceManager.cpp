#include "ResourceManager.h"

ResourceManager::ResourceManager()
{
	loadTexture("Loading Screen", "Loading Screen/Loading Screen.png");
}

void ResourceManager::loadTextures()
{
	//For good practice, we should use the name of the files as the id for the texture
	//For example, "Player" should be the id for "Player.png", this makes it easy to debug
	//And set textures

	//Load game related textures (player, ai, worker etc.)
	loadTexture("Player", "Ships/Player.png", true);
	loadTexture("Player Bullet", "Ships/Player Bullet.png", true);
	loadTexture("Sweeper", "Ships/Sweeper Bot.png", true);
	loadTexture("Worker", "Ships/Worker.png", true);

	//Load minimap textures
	loadTexture("Full Map", "Environment/Full Map.png", true);

	//Load environment Textures (corridors, walls, backgrounds etc)
	loadTexture("Starfield BG", "Environment/Starfield BG.png", true);
	loadTexture("SC", "Environment/Straight Corridor.png", true);
	loadTexture("CC", "Environment/Corridor Corner.png", true);
	loadTexture("TJC", "Environment/Corridor T Junction.png", true);
	loadTexture("CFJ", "Environment/Corrdior 4 Junction.png", true);
	loadTexture("3ER", "Environment/Three Exit Room.png", true);
	loadTexture("TLCR", "Environment/Top Left Corner Room.png", true);
	loadTexture("TER", "Environment/Two Exit Room.png", true);
	loadTexture("ROE", "Environment/Room One Exit.png", true);
	loadTexture("BRFE", "Environment/Big Room Four Exits.png", true);
	loadTexture("RFE", "Environment/Room Four Exits.png", true);
	loadTexture("Worker Area", "Environment/Worker Area.png", true);

	//Others
	loadTexture("Door", "Environment/Door.png", true);
	loadTexture("Spawn Point", "Environment/Spawn Point.png", true);

	finishedLoading = true;
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
