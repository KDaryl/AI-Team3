#include "LevelLoader.h"

LevelLoader::LevelLoader()
{
	//Load in data to our string "content"
	std::ifstream ifs(filePath);
	std::string content((std::istreambuf_iterator<char>(ifs)),
		(std::istreambuf_iterator<char>()));

	//Parse from string to JSON
	data = json::parse(content);

	//Output the data
	std::cout << data.dump() << std::endl;
}
