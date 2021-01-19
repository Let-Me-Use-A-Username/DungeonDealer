#pragma once

#include <list>
#include "GameMap.h"
#include "graphics.h"

class Menu
{
	graphics::Brush br;
	graphics::Brush text;

	bool pressedLoadGame;
	bool pressedExitGame;

public:
	bool loadGame();
	bool exitGame();
	std::string mapLoaded();

	bool setPressedLoadGame() { return pressedLoadGame; }
	bool setPressedExitGame() { return pressedExitGame; }

	bool getPressedLoadGame() { return pressedLoadGame; }
	bool getPressedExitGame() { return pressedExitGame; }

	void update();
	void draw();
};