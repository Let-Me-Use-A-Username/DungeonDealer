#pragma once

struct Square {
	float centerX, centerY;
	float width, height;
};

struct Disk {
	float centerX, centerY;
	float radius;
};

class Collidable {

public:
	virtual Square getCollisionHull() = 0;
	virtual Square attackRange() = 0;
};