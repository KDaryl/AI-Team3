#pragma once
#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"
#include <iostream>

/**
* Description: Resource Manager, this is a central location for resources for the game
*/
class ResourceManager
{
public:
	ResourceManager();
	~ResourceManager() {};
	void loadTextures();
	void loadTexture(std::string name, std::string fileName, bool smooth = false);
	void loadAudio(std::string name, std::string fileName);
	void playAudio(std::string name, std::string type, bool loop = false);
	void stopAudio(std::string name, std::string type);
	bool finishedLoading = false;

	//Getters
	sf::Texture& getTexture(std::string textureName);
	sf::Music& getMusic(std::string musicName);
	sf::Sound& getSound(std::string soundName);
private:
	const std::string m_filePath = "./Resources/"; //All resources will be in this file path
	std::map<std::string, sf::Texture> m_textureMap; //Our texture map
	//music and sound maps 
	std::map<std::string, sf::Music> m_musicMap;
	std::map<std::string, sf::SoundBuffer> m_soundBufferMap;
	std::map<std::string, sf::Sound> m_soundMap;
};

