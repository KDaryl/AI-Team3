#pragma once
#include <iostream>
#include <fstream> //For file reading
#include <nlohmann/json.hpp>
using json = nlohmann::json;


class LevelLoader
{
public:
	LevelLoader();
	~LevelLoader() {}

private:
	const std::string filePath = "./Resources/Game Data.txt"; //Where the data is stored
	json data; //Where our data will be held
};