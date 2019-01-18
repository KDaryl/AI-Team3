#pragma once
#include "SFML/Graphics.hpp"
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

	bool finishedLoading = false;

	//Getters
	sf::Texture& getTexture(std::string textureName);
private:
	const std::string m_filePath = "./Resources/"; //All resources will be in this file path
	std::map<std::string, sf::Texture> m_textureMap; //Our texture map
};
