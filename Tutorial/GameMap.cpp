#include "GameMap.h"

void GameMap::initializeMap(std::string mapName)
{
	EnemyFactory enemyFactory;
	std::list<std::list<int>> data = gameio::readWaveEnemies(mapName);
	int waveCounter = 1;
	for (auto &wave : data)
	{
		std::list<Enemy *> creepList;
		std::string waveName = waveCounter == data.size() ? "final boss" : std::to_string(waveCounter);
		for (auto& code : wave)
		{
			Enemy* enemy = enemyFactory.produceEnemy(code);
			enemy->setPlayer(player);
			creepList.push_back(enemy);
		}
		waves.push_back(new Wave(waveName, creepList));
		waveCounter++;
	}
}

void GameMap::update()
{
	if (player == nullptr) {
		isFinished = true;
		return;
	}

	if (!loadedWaveMusic) {
		std::cout << "Loading music \n";
		graphics::playMusic(std::string(ASSET_PATH_MUSIC) + "DOOM.mp3"	, 1.0f);
		loadedWaveMusic = true;
	}


	player->update();

	if (player->getHp() < 0.1f)
	{
		player->~Player();
		player = nullptr;
	}

	if (waves.size() == 0) {
		graphics::stopMusic();
		isFinished = true;
		return;
	}

	if (waves.front()->waveCleared()){
		Wave * clearedWave = waves.front();
		waves.pop_front();
		clearedWave->~Wave();
	}
	else {
		waves.front()->update();
	}
}

void GameMap::draw()
{
	if (player == nullptr) {
		return;
	}

	backGround.texture = std::string(ASSET_PATH_BACKGROUNDS) + "nightTown.png";
	backGround.outline_opacity = 0.0f;

	graphics::drawRect(CANVAS_WIDTH / 2, CANVAS_HEIGHT / 2, CANVAS_WIDTH, CANVAS_HEIGHT, backGround);

	//Player HP and Health bar
	float playerHP = player->getHp();
	hp.outline_opacity = 0.0f;
	hp.fill_color[0] = 1.0f;
	hp.fill_color[1] = 0.2f;
	hp.fill_color[2] = 0.2f;
	hp.texture = "";
	hp.fill_secondary_color[0] = 0.2f;
	hp.fill_secondary_color[1] = 0.2f;
	hp.fill_secondary_color[2] = 1.0f;
	hp.gradient = true;
	hp.gradient_dir_u = 1.0f;
	hp.gradient_dir_v = 0.0f;

	//remove life from here
	graphics::drawRect(CANVAS_WIDTH - 100 , 30, playerHP * 12, 20, hp);
	//if(player) 
	//	std::cout << "Players HP" << player->getHp();
	
	hp.outline_opacity = 1.0f;
	hp.gradient = false;
	hp.fill_opacity = 0.0f;
	graphics::drawRect(CANVAS_WIDTH - 100 , 30, 120, 20, hp);

	//Floor tileset
	
	tiles.texture = std::string(ASSET_PATH_TILESETS) + "bricks.png";
	tiles.outline_opacity = 0.0f;
	
	graphics::drawRect(510, 618, 1024, 400, tiles);

	player->draw();
	if(waves.size() != 0) waves.front()->draw();
}