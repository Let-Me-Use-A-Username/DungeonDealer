#include "wave.h"

void Wave::initializeWave(std::list<Enemy*> creeps)
{

	for (auto& creep : creeps) {
		creepSpawned++;
		creep->setID(creepSpawned);
		this->creeps.insert(std::pair<int, Enemy *>(creepSpawned, creep));
	}
}

bool Wave::checkCreepLife(Enemy* creep)
{
	if (creep->getHp() < 0.1f) {
		return false;
	}
	return true;
}

void Wave::update() {
	for (auto& creepItem : creeps) {
		if (!checkCreepLife(creepItem.second))
		{
			int id = creepItem.first;
			creepItem.second->remove();
			creepItem.second->~Enemy();
			creeps.erase(id);
		}
		else {
			creepItem.second->update();
		}
	}
}


void Wave::draw() {
	for (auto& creepItem : creeps) {
		creepItem.second->draw();
	}
}