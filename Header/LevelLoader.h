#pragma once
#include <iostream>
#include <fstream> //For file reading
#include <nlohmann/json.hpp>
using json = nlohmann::json;

/**
* Description: This loads and stores the data from a json file
*/
class LevelLoader
{
public:
	LevelLoader();
	~LevelLoader() {}

	json data; //Where our data will be held
private:
	const std::string filePath = "./Resources/Game Data.txt"; //Where the data is stored
};