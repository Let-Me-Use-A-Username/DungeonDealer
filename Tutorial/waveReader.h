#ifndef WAVEREADER_H
#define WAVEREADER_H
#pragma once

#include <fstream>
#include <string>
#include <iostream>
#include <list>

namespace gameio {

	std::list<std::list<int>> readWaveEnemies(std::string mapName);

	void enemyCodeReader(std::string codes);
}

#endif