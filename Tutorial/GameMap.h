#ifndef GAMEMAP_H
#define GAMEMAP_H
#pragma once

#include "wave.h"
#include "waveReader.h"
#include "graphics.h"

class GameMap {
	
	std::list<Wave *> waves;

	graphics::Brush backGround;
	graphics::Brush hp;
	graphics::Brush tiles;

	bool loadedWaveMusic = false;

	bool isFinished = false;

	Player * player;

	void initializeMap(std::string mapName);

public:


	GameMap(std::string mapName)
	{
		player = new Player();
		initializeMap(mapName);
		for (auto& wave : waves)
		{
			wave->out();
		}
	}

	~GameMap() 
	{
		waves.clear();
		if(player != nullptr) player->~Player();
	}

	void update();

	void draw();

	bool finishedMap() { return isFinished; }
};

#endif