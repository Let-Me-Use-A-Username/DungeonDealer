#include "player.h"

void Player::update()
{
	time_t currentTime;
	time(&currentTime);

	if (invunerable) {
		if (currentTime - invunerableTimer > 3)
		{
			setInvunerable(false);
		}
	}

	//velocity += gravity * graphics::getDeltaTime();
	velocity = gravity * graphics::getDeltaTime() / 10;

	//std::cout << "Velocity is " << velocity << "\n";

	if (graphics::getKeyState(graphics::SCANCODE_LEFT)) {
		pos_x -= speed * graphics::getDeltaTime() / 10.0f;
		gameFrame += 1;
		// Every 10 frames the next running png photo loads.
		if (gameFrame == 10) {
			runningFrame++;
			gameFrame = 0;
		}
		isWalking = true;
		direction = -1;
	}
	if (graphics::getKeyState(graphics::SCANCODE_RIGHT)) {
		pos_x += speed * graphics::getDeltaTime() / 10.0f;
		gameFrame += 1;
		// Every 10 frames the next running png photo loads.
		if (gameFrame == 10) {
			runningFrame++;
			gameFrame = 0;
		}
		isWalking = true;
		direction = 1;
	}
	if (graphics::getKeyState(graphics::SCANCODE_UP)) {
		//Do nothing
	}
	if (graphics::getKeyState(graphics::SCANCODE_DOWN)) {
		isCrouching = true;
	}

	if (isAirborne) {
		//going up
		if (isGoingUp && !reachedUpperJumpBound()) {
			setPosY(pos_y + JUMP_STEP);
			speed = 8.0f;
		}
		// falling
		else if (!isGoingUp && pos_y < posYpreJump) {
			setPosY(pos_y - JUMP_STEP);
			//reached ground
			if (pos_y >= posYpreJump) {
				setPosY(posYpreJump);
				isAirborne = false;
				speed = 3.0f;
			}
		}

		if (reachedUpperJumpBound()) {
			isGoingUp = false;
		}

	}

	if (graphics::getKeyState(graphics::SCANCODE_SPACE)) {
		if (!isAirborne) {
			isAirborne = true;
			isGoingUp = true;
			posYpreJump = pos_y;
		}
	}

	if (graphics::getKeyState(graphics::SCANCODE_LCTRL)) {
		isAttacking = true;
	}
	if (graphics::getKeyState(graphics::SCANCODE_X)) {
		//execute attack
	}

	if (pos_x < 0) pos_x = 0;
	if (pos_x > CANVAS_WIDTH) pos_x = CANVAS_WIDTH;	
	if (pos_y < 0) pos_y = 0;
	if (pos_y > CANVAS_HEIGHT) pos_y = CANVAS_HEIGHT;
}

void Player::draw()
{

	isIdle = true;

	if (runningFrame > 11) { runningFrame = 0; }

	if (idleFrame > 3) { idleFrame = 0; }

	if (attackFrame > 5) { attackFrame = 0; }

	if (isAttacking) {
		isIdle = false;
		attack();
	}

	if (isWalking) {
		isIdle = false;
		walk();
	}

	if (isCrouching) {
		isIdle = false;
		crouch();
	}

	if (!isWalking && !isAttacking && !isCrouching && isIdle) {
		idle();
	}
}

//Function that loads the crouch sprite. (I couldn't somehow slow down the loading proccess of the sprites
//so I only load the last one.
void Player::crouch() {
	if (direction == -1) {
		br.texture = std::string(ASSET_PATH_PLAYER) + "playerCrouchLeft\\2.png";
	}
	else {
		br.texture = std::string(ASSET_PATH_PLAYER) + "playerCrouch\\2.png";
	}
	br.outline_opacity = 0.0f;
	graphics::drawRect(pos_x, pos_y + 170, 48, 48, br);

	isCrouching = false;
}

//Function that loads the attack sprites.
void Player::attack() {
	float currentTime = graphics::getDeltaTime();
	//TODO: Fix the attack animation
	if (direction == -1) {
		br.texture = std::string(ASSET_PATH_PLAYER) + "playerAttackLeft\\" + std::to_string(attackFrame) + ".png";
	}
	else {
		br.texture = std::string(ASSET_PATH_PLAYER) + "playerAttack\\" + std::to_string(attackFrame) + ".png";
	}
	
	br.outline_opacity = 0.0f;
	graphics::drawRect(pos_x, pos_y + 170, 66, 48, br);

	attackFrame++;
	isAttacking = false; 
}

//Function that loads the walking sprites.
void Player::walk() {
	if (direction == -1) {
		br.texture = std::string(ASSET_PATH_PLAYER) + "playerRunLeft\\" + std::to_string(runningFrame) + ".png";
	}
	else {
		br.texture = std::string(ASSET_PATH_PLAYER) + "playerRun\\" + std::to_string(runningFrame) + ".png";
	}
			
	br.outline_opacity = 0.0f;

	graphics::drawRect(pos_x, pos_y + 170, 66, 48, br);

	isWalking = false;
}

//Function that loads the idle sprites.
void Player::idle() {
	//TODO: Fix the idle animation
	if (direction == -1) {
		br.texture = std::string(ASSET_PATH_PLAYER) + "playerIdleLeft\\" + std::to_string(idleFrame) + ".png";
	}
	else {
		br.texture = std::string(ASSET_PATH_PLAYER) + "playerIdle\\" + std::to_string(idleFrame) + ".png";
	}
	
	br.outline_opacity = 0.0f;

	graphics::drawRect(pos_x, pos_y + 170, 38, 48, br);

	idleFrame++;
}

void Player::damageHP(float damage) {
	hp = std::max(0.0f, hp - damage);
	//hurt();
}

//Function returns the players collition range in form of square coordinates.
Square Player::getCollisionHull()
{
	Square playerRect;

	playerRect.centerX = pos_x ;
	playerRect.centerY = pos_y + 175;
	playerRect.height = 39.0f;
	playerRect.width = 17.0f;

	return playerRect;
}

//Function returns the players attack range in form of disk coordinates.
Square Player::attackRange()
{
	Square attackRange;

	if (direction == -1) {
		attackRange.centerX = pos_x - 30;
		attackRange.centerY = pos_y + 170;
	}
	else {
		attackRange.centerX = pos_x + 30;
		attackRange.centerY = pos_y + 170;
	}
	attackRange.height = 30;
	attackRange.width = 55;
	return attackRange;
}
