#include "waveReader.h"


std::list<std::list<int>> gameio::readWaveEnemies(std::string mapName)
{

	std::ifstream mapFile(mapName);
	std::string lineRead;
	std::string delimiter = " ";
	int counter = 1;
	std::list<std::list<int>> map;

	while (getline(mapFile, lineRead)) {
		std::string token;
		size_t pos = 0;
		std::list<int> wave;
		while ((pos = lineRead.find(delimiter)) != std::string::npos) {
			wave.push_back(stoi(lineRead.substr(0, pos)));
			lineRead.erase(0, pos + delimiter.length());
		}
		pos = lineRead.find("\n");
		if (lineRead.length() > 0) wave.push_back(stoi(lineRead.substr(0, pos)));
		map.push_back(wave);
	}

	mapFile.close();
	return map;
}

void gameio::enemyCodeReader(std::string codes)
{
	std::ifstream codeFile(codes);
	std::string lineRead;
	

	codeFile.close();
}
