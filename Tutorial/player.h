#pragma once
#include <iostream>
#include "config.h"
#include "graphics.h"
#include "gameObject.h"
#include "Collidable.h"
#include <time.h>

class Player : public GameObject, public Collidable
{
	graphics::Brush br;

	//Variables that load the next png for the animations.

	// direction: 1 = right, -1 = left, 0 undefined
	int direction = 0;
	int idleFrame = 0;
	int gameFrame = 0;
	int attackFrame = 0;
	int runningFrame = 0;
	float velocity = 0;

	bool isIdle = true;
	bool isWalking = false;
	float JUMP_STEP = -6.0f;
	bool isAirborne = false;

	int UPPER_JUMP_HEIGHT = -150;
	bool reached_upper_jump_bound = false;
	bool isGoingUp = false;
	float posYpreJump = 0.0f;
	
	bool isCrouching = false;

	bool invunerable = false;
	time_t invunerableTimer;

	bool collition = true;

	float hp = 10.0f;
	float speed = 3.0f;
	float gravity = 0.9f;
	float swingDamage = 10.0f;
	float pos_x = CANVAS_WIDTH / 2, pos_y = CANVAS_HEIGHT / 2;


public: 
	bool isAttacking = false;

	Player(){}
	void update() override;
	void draw() override;

	void crouch();
	void idle();
	void walk();
	void attack();
	
	bool canCollide() { return collition; }

	bool isInvulnerable() { return invunerable; }
	void setInvunerable(bool inv) { invunerable = inv; }
	void makeInvulnerable() { 
		invunerable = true;
		time(&invunerableTimer);
	}

	float getHp() { return hp; }
	void damageHP(float damage);

	float getPlayerDamage() { return swingDamage; }

	void setPosX(float pos) { pos_x = pos; }
	void setPosY(float pos) { pos_y = pos; }
	float getPosX() { return pos_x; }
	float getPosY() { return pos_y + 170; }

	float getPosYPreJump() { return posYpreJump; }
	bool reachedUpperJumpBound() {
		return pos_y <= posYpreJump + UPPER_JUMP_HEIGHT;
	}

	Square getCollisionHull() override;
	Square attackRange() override;

	float getVelocity() { return gravity* graphics::getDeltaTime() / 10; }
};