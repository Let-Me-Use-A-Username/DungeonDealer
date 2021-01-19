#ifndef ENEMY_H
#define ENEMY_H
#pragma once

#include "config.h"
#include "graphics.h"
#include "player.h"
#include <random>
#include "Collidable.h"


class Enemy : public GameObject, public Collidable {
protected:

	graphics::Brush  walkState;
	graphics::Brush attackState;
	
	float spawnTime;

	Player * player;

	int id = 0;
	float hp = 0;
	float size = 0;
	float speed = 0;
	float damage = 0;

	float pos_x = 0;
	float pos_y = 0;
	int direction = 0;

	bool attack = false;
	bool hasDefaultAttack = true;

	int gameFrame = 0;
	int walkingFrame = 0;

public:

	virtual void update() = 0;
	virtual void draw() = 0;

	virtual float getCollitionDamage() = 0;
	float getAttackDamage()
	{
		if (!hasDefaultAttack) {
			return 0.0f;
		}
		else {
			return damage;
		}

	}

	float getHp() { return hp; }
	void damageHP(float damage) { hp -= damage; }

	bool defaultAttack() { return hasDefaultAttack; }

	Square getCollisionHull() { return Square(); };
	Square attackRange() { return Square(); };

	float getPosX() { return pos_x; }
	float getPosY() { return pos_y; }
	void setPosX(float newX) { pos_x = newX; }
	void setPosY(float newY) { pos_y = newY; }

	void setDirection(int dir) { direction = dir; }
	int getDirection() { return direction; }

	int getID() { return id; }
	void setID(int id) { this->id = id; }

	float getSpawnTime() { return spawnTime; }
	void setSpawnTime(float spawnTime) { this->spawnTime = spawnTime; }

	bool isAttacking() { return attack; }
	void setAttack(bool attackState) { attack = attackState; }

	void setPlayer(Player * player) { this->player = player; }

	//Collition check for player and mob 
	bool checkCollision(Player * player)
	{
		Square playerRect = player->getCollisionHull();
		Square enemyRect = getCollisionHull();

		if (playerRect.centerX < enemyRect.centerX + enemyRect.width &&
			playerRect.centerX + playerRect.width > enemyRect.centerX &&
			playerRect.centerY < enemyRect.centerY + enemyRect.width &&
			playerRect.centerY + playerRect.height > enemyRect.centerY) {
			if (!player->isInvulnerable()) {
				player->damageHP(getCollitionDamage());
				player->makeInvulnerable();
			}

			return true;
		}

		return false;
	}

	void remove() {

		walkState.fill_opacity = 0.0f;
		attackState.fill_opacity = 0.0f;
	}
	
	//Checks whether the mobs default attack is in range.
	bool checkMobAttackRange(Player* player)
	{
		Square playerRect = player->getCollisionHull();
		Square enemyRect = attackRange();

		if (playerRect.centerX < enemyRect.centerX + enemyRect.width &&
			playerRect.centerX + playerRect.width > enemyRect.centerX &&
			playerRect.centerY < enemyRect.centerY + enemyRect.width &&
			playerRect.centerY + playerRect.height > enemyRect.centerY) {
			
			setAttack(true);
			return true;
		}
		return false;
	}

	//Colition check for players attack and mobs collition square
	bool checkAttackRange(Player* player) {
		Square attackRange = player->attackRange();
		Square enemyRect = getCollisionHull();

		if (player->isAttacking)
		{
			if (attackRange.centerX < enemyRect.centerX + enemyRect.width &&
				attackRange.centerX + attackRange.width > enemyRect.centerX &&
				attackRange.centerY < enemyRect.centerY + enemyRect.width &&
				attackRange.centerY + attackRange.height > enemyRect.centerY) {
				damageHP(player->getPlayerDamage());
				return true;
			}
		}
		return false;
	}

	void correctDirection(Player* player)
	{
		float mobX = this->getPosX();
		float playerX = player->getPosX();

		if (mobX < playerX) {
			if (this->getDirection() == -1) {
				this->setDirection(1);
			}
		}
		else if (mobX > playerX) {
			if (this->getDirection() == 1) {
				this->setDirection(-1);
			}
		}
	}

	virtual std::string out() = 0;
};

class HellBeast : public Enemy {

	int attackingFrame = 0;
	int attackFrameTimer = 0;

public:

	HellBeast() {
		hp = 10.0f;
		size = 50.0f;
		speed = 0.7f;
		damage = 3.0f;
		pos_x = CANVAS_WIDTH / 2 + (rand() % 1200 - 600);
		pos_y = CANVAS_HEIGHT / 2 + 170;
		direction = 1;
		gameFrame = 0;
		walkingFrame = 0;
	}

	void update() {
		if (direction == -1) {
			
			if (attack) {
				pos_x -= speed * graphics::getDeltaTime() / 20;
			}
			else {
				pos_x -= speed * graphics::getDeltaTime() / 10;
			}
		}
		else {
			if (attack) {
				pos_x += speed * graphics::getDeltaTime() / 20;
			}
			else {
				pos_x += speed * graphics::getDeltaTime() / 10;
			}
		}

		gameFrame++;
		if (gameFrame == 15) {
			walkingFrame++;
			gameFrame = 0;
		}

		attackFrameTimer++;
		if (attackFrameTimer == 70) {
			attackingFrame++;
			attackFrameTimer = 0;
		}

		if (pos_x < 0) pos_x = 0;
		if (pos_x > CANVAS_WIDTH)  pos_x = CANVAS_WIDTH;
		if (pos_y < 0) pos_y = 0;
		if (pos_y > CANVAS_HEIGHT) pos_y = CANVAS_HEIGHT;

		
		checkCollision(player);
		checkAttackRange(player);
		correctDirection(player);
		checkMobAttackRange(player);
	}

	void draw()
	{

		if (walkingFrame > 5) { walkingFrame = 0; }

		if (attackingFrame > 5) {
			attackingFrame = 0;
			attack = false;
		}

		if (!attack) {
			if (direction == -1) {
				walkState.texture = std::string(ASSET_PATH_HELL_BEAST) + "walking\\" + std::to_string(walkingFrame) + ".png";
			}
			else {
				walkState.texture = std::string(ASSET_PATH_HELL_BEAST) + "walkingLeft\\" + std::to_string(walkingFrame) + ".png";
			}
		}

		walkState.outline_opacity = 0.0f;
		graphics::drawRect(pos_x, pos_y, size, size, walkState);

		if (attack) {
			if (direction == -1) {
				attackState.texture = std::string(ASSET_PATH_HELL_BEAST) + "attack\\" + std::to_string(attackingFrame) + ".png";
			}
			else {
				attackState.texture = std::string(ASSET_PATH_HELL_BEAST) + "attackLeft\\" + std::to_string(attackingFrame) + ".png";
			}
			attackState.outline_opacity = 0.0f;
			if (attackingFrame < 4) {
				graphics::drawRect(pos_x, pos_y, 59, 57, attackState);
			}
			else {
				graphics::drawRect(pos_x, pos_y - 53, 70, 160, attackState);
			}
		}
	}

	float getCollitionDamage()
	{
		return 2.0f;
	}

	Square getCollisionHull()
	{
		Square collition;
		//Maybe make the hellbeast spawn random and dont move them.
		//Since its the only mob with a ranged and melee attack.
		collition.centerX = pos_x;
		collition.centerY = pos_y + 10;
		collition.height = size - 10;
		collition.width = size - 25;

		return collition;
	}

	Square attackRange()
	{
		Square range;

		range.centerX = pos_x;
		range.centerY = pos_y + 10;
		range.height = size + 20;
		range.width = size + 10;

		return range;
	}

	std::string out() {
		return "hellbeast - key: " + std::to_string(id);
	}
};

class Nightmare : public Enemy 
{
public:

	Nightmare() : Enemy() {
		hp = 20.0;
		size = 80.0;
		speed = 3.0f;
		damage = 0.0f;
		pos_x = CANVAS_WIDTH / 2 + (rand() % 1200 - 600);
		pos_y = CANVAS_HEIGHT / 2 + 154;
		direction = -1;
		gameFrame = 0;
		walkingFrame = 0;
	}
	void update()
	{
		if (direction == -1) {
			pos_x -= speed * graphics::getDeltaTime() / 10;
		}
		else {
			pos_x += speed * graphics::getDeltaTime() / 10;
		}

		gameFrame++;
		if (gameFrame == 15) {
			walkingFrame++;
			gameFrame = 0;
		}

		if (pos_x < 0) {
			pos_x = 0;
			direction = 1;
		}
		if (pos_x > CANVAS_WIDTH) {
			pos_x = CANVAS_WIDTH;
			direction = -1;
		}
		if (pos_y < 0) pos_y = 0;
		if (pos_y > CANVAS_HEIGHT) pos_y = CANVAS_HEIGHT;

		checkCollision(player);
		checkAttackRange(player);
	}

	void draw()
	{

		if (walkingFrame > 3) { walkingFrame = 0; }

		if (direction == -1) {
			walkState.texture = std::string(ASSET_PATH_NIGHTMARE) + "galoping\\" + std::to_string(walkingFrame) + ".png";
		}
		else {
			walkState.texture = std::string(ASSET_PATH_NIGHTMARE) + "galopingLeft\\" + std::to_string(walkingFrame) + ".png";
		}

		walkState.outline_opacity = 0.0f;
		graphics::drawRect(pos_x, pos_y, size + 40, size, walkState);
	}

	Square getCollisionHull()
	{
		Square collition;

		collition.centerX = pos_x;
		collition.centerY = pos_y + 16;
		collition.height = size - 30;
		collition.width = size - 30;

		return collition;
	}

	Square attackRange()
	{
		Square attackRange;

		attackRange.centerX = pos_x;
		attackRange.centerY = pos_y + 16;
		attackRange.height = size - 30;
		attackRange.width = size - 30;

		return attackRange;
	}

	float getCollitionDamage() {
		return 2.0f;
	}

	std::string out() {
		return "nightmare - key: " + std::to_string(id);
	}
};

class FireSkull : public Enemy
{

	int directionY;

public:

	FireSkull() : Enemy()
	{
		hp = 5.0f;
		size = 40;
		speed = 0.3f;
		damage = 0.0f;
		pos_x = CANVAS_WIDTH / 2 + (rand() % 1200 - 600) ;
		pos_y = CANVAS_HEIGHT / 2 + rand() % 100;
		direction = 1;
		directionY = 1;
		gameFrame = 0;
		walkingFrame = 0;
	}

	void update()
	{
		if (direction == -1) {
			pos_x -= speed * graphics::getDeltaTime() / 10;

		}
		else if (direction == 1) {
			pos_x += speed * graphics::getDeltaTime() / 10;

		}
		else {
			pos_x = pos_x;
		}

		if (directionY == -1) {
			pos_y -= speed * graphics::getDeltaTime() / 10;
		}
		else if (directionY == 1) {
			pos_y += speed * graphics::getDeltaTime() / 10;
		}
		else {
			pos_y = pos_y;
		}

		gameFrame++;
		if (gameFrame == 20) {
			walkingFrame++;
			gameFrame = 0;
		}

		if (pos_x < 0) pos_x = 0;
		if (pos_x > CANVAS_WIDTH) pos_x = CANVAS_WIDTH;
		if (pos_y < 0) pos_y = 0;
		if (pos_y > CANVAS_HEIGHT) pos_y = CANVAS_HEIGHT;

		checkCollision(player);
		correctDirection(*player);
		checkAttackRange(player);
	}

	void draw()
	{

		if (walkingFrame > 7) { walkingFrame = 0; }

		if (direction == -1) {
			walkState.texture = std::string(ASSET_PATH_FIRESKULL) + "walking\\" + std::to_string(walkingFrame) + ".png";
		}
		else {
			walkState.texture = std::string(ASSET_PATH_FIRESKULL) + "walkingLeft\\" + std::to_string(walkingFrame) + ".png";
		}

		walkState.outline_opacity = 0.0f;
		graphics::drawRect(pos_x, pos_y, size, size, walkState);
	}

	float getCollitionDamage()
	{
		return 2.5f;
	}

	float getAttackDamage()
	{
		if (!hasDefaultAttack) {
			return 0.0f;
		}
		else {
			return damage;
		}
	}

	Square getCollisionHull()
	{
		Square collition;

		collition.centerX = pos_x;
		collition.centerY = pos_y + 5;
		collition.height = size - 25;
		collition.width = size - 25;

		return collition;
	}

	Square attackRange()
	{
		Square collition;

		collition.centerX = pos_x;
		collition.centerY = pos_y + 5;
		collition.height = size - 25;
		collition.width = size - 25;

		return collition;
	}

	void setDirectionY(int dir)
	{
		directionY = dir;
	}

	int getDiretionY() { return directionY; }
	
	
	// directionX: 1 = right, -1 = left, 0 undefined
	// directionY: 1 = down,  -1 = up,   0 undefined
	void correctDirection(Player& player) {
	float mobX = this->getPosX();
	float mobY = this->getPosY();

	float playerX = player.getPosX();
	float playerY = player.getPosY();

	std::cout << "Mob's X and Y " << mobX << " " << mobY <<"\n";
	std::cout << "players's X and Y " << playerX << " " << playerY << "\n";


	if (mobX < playerX) {
		std::cout << "M0B SMALLER X THAN PLAYER" << "\n";
		if (this->getDirection() == -1) {
			this->setDirection(1);
		}
	}
	else if (mobX > playerX) {
		std::cout << "M0B BIGGER X THAN PLAYER" << "\n";
		if (this->getDirection() == 1) {
			this->setDirection(-1);
		}
	}

	if (mobY < playerY) {
		std::cout << "M0B SMALLER Y THAN PLAYER" << "\n";
		if (this->getDiretionY() == -1) {
			this->setDirectionY(1);
		}
	}
	else if (mobY > playerY) {
		std::cout << "M0B BIGGER Y THAN PLAYER" << "\n";
		if (this->getDiretionY() == 1) {
			this->setDirectionY(-1);
		}
	}
}


	std::string out() {
		return "fireskull - key: " + std::to_string(id);
	}
};

class Reaper : public Enemy
{
	int attackingFrame = 0;
	int attackFrameTimer = 0;

public:

	Reaper() : Enemy()
	{
		hp = 200.0f;
		size = 50.0f;
		speed = 1.8f;
		damage = 4.0f;
		pos_x = CANVAS_WIDTH / 2 + (rand() % 400 - 200);
		pos_y = CANVAS_HEIGHT / 2 + 170;
		direction = 1;
		gameFrame = 0;
		walkingFrame = 0;
	}

	void update()
	{
		if (direction == -1) {
			if (!attack) {
				pos_x -= speed * graphics::getDeltaTime() / 10;
			}
		}
		else {
			if (!attack) {
				pos_x += speed * graphics::getDeltaTime() / 10;
			}
		}

		gameFrame++;
		if (gameFrame == 15) {
			walkingFrame++;
			gameFrame = 0;
		}

		attackFrameTimer++;
		if (attackFrameTimer == 10) {
			attackingFrame++;
			attackFrameTimer = 0;
		}

		if (pos_x < 0) pos_x = 0;
		if (pos_x > CANVAS_WIDTH)  pos_x = CANVAS_WIDTH;
		if (pos_y < 0) pos_y = 0;
		if (pos_y > CANVAS_HEIGHT) pos_y = CANVAS_HEIGHT;

		checkMobAttackRange(player);
		checkCollision(player);
		checkAttackRange(player);
		correctDirection(player);
	}

	void draw()
	{

		if (walkingFrame > 4) { walkingFrame = 0; }

		if (attackingFrame > 9) { 
			attack = false; 
			attackingFrame = 0;
		}

		if (!attack) {
			if (direction == 1) {
				walkState.texture = std::string(ASSET_PATH_REAPER) + "walking\\" + std::to_string(walkingFrame) + ".png";
			}
			else {
				walkState.texture = std::string(ASSET_PATH_REAPER) + "walkingLeft\\" + std::to_string(walkingFrame) + ".png";
			}
		}
		walkState.outline_opacity = 0.0f;
		graphics::drawRect(pos_x, pos_y, size, size, walkState);

		if (attack) {

			if (direction == -1) {
				attackState.texture = std::string(ASSET_PATH_REAPER) + "attack\\" + std::to_string(attackingFrame) + ".png";
			}
			else {
				attackState.texture = std::string(ASSET_PATH_REAPER) + "attackLeft\\" + std::to_string(attackingFrame) + ".png";
			}
			attackState.outline_opacity = 0.0f;
			graphics::drawRect(pos_x, pos_y, size, size, attackState);
		}
	}

	float getCollitionDamage()
	{
		return 2.0f;
	}

	Square getCollisionHull()
	{
		Square collition;

		collition.centerX = pos_x + 3;
		collition.centerY = pos_y;
		collition.width = size - 35;
		collition.height = size - 20;

		return collition;
	}

	Square attackRange()
	{
		Square range;

		range.centerX = pos_x + 30;
		range.centerY = pos_y;
		range.width = size - 35;
		range.height = size - 20;

		return range;
	}

	std::string out() {
		return "reaper - key: " + std::to_string(id);
	}


};

class Demon : public Enemy
{
	int attackingFrame = 0;
	int attackFrameTimer = 0;

public:

	Demon() : Enemy()
	{
		hp = 200.0f;
		size = 140.0f;
		speed = 1.8f;
		damage = 4.0f;
		pos_x = CANVAS_WIDTH / 2 + (rand() % 400 - 200);
		pos_y = CANVAS_HEIGHT / 2 + 110;
		direction = 1;
		gameFrame = 0;
		walkingFrame = 0;
	}

	void update()
	{
		if (direction == -1) {
			if (!attack) {
				pos_x -= speed * graphics::getDeltaTime() / 10;
			}
		}
		else {
			if (!attack) {
				pos_x += speed * graphics::getDeltaTime() / 10;
			}
		}

		gameFrame++;
		if (gameFrame == 30) {
			walkingFrame++;
			gameFrame = 0;
		}

		attackFrameTimer++;
		if (attackFrameTimer == 30) {
			attackingFrame++;
			attackFrameTimer = 0;
		}

		if (pos_x < 0) pos_x = 0;
		if (pos_x > CANVAS_WIDTH)  pos_x = CANVAS_WIDTH;
		if (pos_y < 0) pos_y = 0;
		if (pos_y > CANVAS_HEIGHT) pos_y = CANVAS_HEIGHT;

		checkMobAttackRange(player);
		checkCollision(player);
		checkAttackRange(player);
		correctDirection(player);
	}

	void draw()
	{

		if (walkingFrame > 5) { walkingFrame = 0; }

		if (attackingFrame > 10) { 
			attack = false; 
			attackingFrame = 0; 
		}

		if (!attack) {
			if (direction == -1) {
				walkState.texture = std::string(ASSET_PATH_DEMON) + "walking\\" + std::to_string(walkingFrame) + ".png";
			}
			else {
				walkState.texture = std::string(ASSET_PATH_DEMON) + "walkingLeft\\" + std::to_string(walkingFrame) + ".png";
			}
		}
		walkState.outline_opacity = 0.0f;
		graphics::drawRect(pos_x, pos_y, size + 20, size, walkState);

		if (attack) {

			if (direction == -1) {
				attackState.texture = std::string(ASSET_PATH_DEMON) + "attack\\" + std::to_string(attackingFrame) + ".png";
			}
			else {
				attackState.texture = std::string(ASSET_PATH_DEMON) + "attackLeft\\" + std::to_string(attackingFrame) + ".png";
			}
			attackState.outline_opacity = 0.0f;
			walkState.fill_opacity = 0.0f;
			walkState.texture = "";
			graphics::drawRect(pos_x, pos_y, size + 100 , size + 50, attackState);
		}
	}

	float getCollitionDamage()
	{
		return 2.0f;
	}

	Square getCollisionHull()
	{
		Square collition;

		collition.centerX = pos_x + 3;
		collition.centerY = pos_y;
		collition.width = size - 35;
		collition.height = size - 20;

		return collition;
	}

	Square attackRange()
	{
		Square range;

		range.centerX = pos_x + 30;
		range.centerY = pos_y;
		range.width = 70;
		range.height = 100;

		return range;
	}

	std::string out() {
		return "demon - key: " + std::to_string(id);
	}


};

class EnemyFactory {

public:

	Enemy* produceEnemy(int code) {

		Enemy* product = nullptr;

		switch (code) {
		case 1:
			product = new HellBeast();
			break;
		case 2:
			product = new Nightmare();
			break;
		case 3:
			product = new Reaper();
			break;
		case 4:
			product = new FireSkull();
			break;
		case 5:
			product = new Demon();
			break;
		}

		return product;
	}

};

#endif