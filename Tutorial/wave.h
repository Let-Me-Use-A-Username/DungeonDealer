#ifndef WAVE_H
#define WAVE_H
#pragma once

#include <map>
#include <list>
#include <random>
#include <iostream>
#include "Enemy.h"

class Wave
{
	std::string waveName;
	int creepSpawned;
	std::map<int, Enemy *> creeps;
	bool waveSpawned;

	void initializeWave(std::list<Enemy *> creeps);

	bool checkCreepLife(Enemy * creep);

public:

	Wave(std::string waveName, std::list<Enemy *> creeps)
	{
		creepSpawned = 0;
		this->waveName = waveName;
		initializeWave(creeps);
	}

	~Wave()
	{
		creeps.clear();
	}

	void out() {
		std::string creepListName;
		std::string divider = "\n-----------------------\n\n";
		
		for (auto & creepItem : creeps) {
			creepListName += "\n" + creepItem.second->out() + ":" + std::to_string(creepItem.first);
		}
		std::cout << divider << "Wave " << waveName << "\n" << creepListName << divider;
	}

	void update();
	void draw();

	bool waveCleared() { return creeps.size() == 0; }
	
};

#endif