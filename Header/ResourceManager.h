#pragma once
#include "SFML/Graphics.hpp"
#include <iostream>

//This is a singleton class so there can only be one of it active

class ResourceManager
{
public:
	ResourceManager(ResourceManager const&) = delete;
	void operator=(ResourceManager const&) = delete;

	static ResourceManager& getInstance();

	void loadTexture(std::string name, std::string fileName);

	//Getters
	sf::Texture& getTexture(std::string textureName);


private:
	ResourceManager();
	~ResourceManager() {};
	const std::string m_filePath = "./Resources/"; //All resources will be in this file path
	std::map<std::string, sf::Texture> m_textureMap; //Our texture map
};
