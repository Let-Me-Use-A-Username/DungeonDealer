#pragma once

#include "graphics.h"
#include "config.h"
#include "GameMap.h"
#include "menu.h"

class Game
{

	GameMap * gameMap;
	Menu* menu;

	bool removedMenu;
	bool mapLoaded;

public:

	Game() {
		menu = new Menu();
		removedMenu = false;
	}

	void update()
	{

		if (gameMap != nullptr) {

			if (gameMap->finishedMap())
			{
				gameMap->~GameMap();
				gameMap = nullptr;
				mapLoaded = false;
			}
		}

		if (!mapLoaded) {
			if(menu != nullptr) menu->update();
		}
		else {
			gameMap->update();
		}

		if (menu != nullptr) {
			if (menu->getPressedLoadGame()) {
				gameMap = new GameMap(menu->mapLoaded());
				mapLoaded = true;
			}
		}
	}

	void draw()
	{
		if (!mapLoaded) {
			if (menu == nullptr) {
				menu = new Menu();
				removedMenu = false;
			}
			menu->draw();
		}
		else {
			if (!removedMenu) {
				removedMenu = true;
				menu->~Menu();
				menu = nullptr;
			}
			gameMap->draw();
		}
	}

	~Game()
	{
		if(gameMap != nullptr) gameMap->~GameMap();
		if (menu != nullptr) menu->~Menu();
	}

	void setMap(GameMap *newGameMap) {
		gameMap = newGameMap;
		mapLoaded = true;
	}

	void removeMap() {
		gameMap->~GameMap();
		mapLoaded = false;
	}

	bool hasMapLoaded() { return mapLoaded; }
};