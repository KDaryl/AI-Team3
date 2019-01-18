#include "ResourceManager.h"

ResourceManager::ResourceManager()
{
	loadTexture("Loading Screen", "Loading Screen/Loading Screen.png");
	//start all volumes at 100
	for (auto& key : m_soundMap)
	{
		m_soundMap[key.first].setVolume(100);
	}
	for (auto& key : m_musicMap)
	{
		m_musicMap[key.first].setVolume(100);
	}
}

void ResourceManager::loadTextures()
{
	//For good practice, we should use the name of the files as the id for the texture
	//For example, "Player" should be the id for "Player.png", this makes it easy to debug
	//And set textures

	//Load music
	m_musicMap["GameMusic"].openFromFile("./Resources/Audio/BackgroundMusic.ogg");
	//Load Sounds
	loadAudio("Move", "Audio/playerMove.wav");
	loadAudio("Shoot", "Audio/shot.wav");

	//Load game related textures (player, ai, worker etc.)
	loadTexture("Player", "Ships/Player.png", true);
	loadTexture("Player Bullet", "Ships/Player Bullet.png", true);
	loadTexture("Sweeper", "Ships/Sweeper Bot.png", true);
	loadTexture("Worker", "Ships/Worker.png", true);
	loadTexture("Predator", "Ships/Predator.png", true);
	loadTexture("Predator Bullet", "Ships/Enemy Bullet.png", true);
	loadTexture("Alien Nest", "Ships/Alien Nest.png", true);
	loadTexture("Nest Missile", "Ships/Nest Missile.png", true);

	//Load minimap textures
	loadTexture("Full Map", "Environment/Full Map.png", true);

	//Hud Textures
	loadTexture("HUD", "HUD/HUD BG.png", true);
	loadTexture("HUD Bar Empty", "HUD/Bar Empty.png", true);
	loadTexture("HUD Bar Full", "HUD/Bar Full.png", true);

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

void ResourceManager::loadAudio(std::string name, std::string fileName)
{
	//load into soundbuffer
	if (!m_soundBufferMap[name].loadFromFile(m_filePath + fileName))
	{
		std::cout << "Cant find file" << std::endl;
	}
	m_soundMap[name].setBuffer(m_soundBufferMap[name]);
}

void ResourceManager::playAudio(std::string name, std::string type, bool loop)
{
	if (type == "Music")
	{
		m_musicMap[name].stop();
		m_musicMap[name].setLoop(loop);
		m_musicMap[name].play();
	}
	else
	{
		m_soundMap[name].stop();
		m_soundMap[name].setLoop(loop);
		m_soundMap[name].play();
	}
}

void ResourceManager::stopAudio(std::string name, std::string type)
{
	if (type == "sound")
	{
		m_musicMap[name].setLoop(false);
		m_musicMap[name].stop();
	}
}

sf::Texture & ResourceManager::getTexture(std::string textureName)
{
	return m_textureMap[textureName];
}

sf::Music & ResourceManager::getMusic(std::string musicName)
{
	return m_musicMap[musicName];
}

sf::Sound & ResourceManager::getSound(std::string soundName)
{
	return m_soundMap[soundName];
}
