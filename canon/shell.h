#pragma once
#include "canon.h"
#include <cmath>
#include <vector>
#include "vec3.h"

extern float shellSpeed;

struct Position {
	float x0, y0, z0;
	float x, y, z;
	float vx, vy, vz;
	float gravity = 9.8f;
	float t;
	bool alive = false;
};

Vec3 computeImpact(Vec3 p0, float v, float pitch, float yaw);

class Shells {
public:
	float s = 0.1f;

	Position pos;
	std::vector<Position> shells;

	void drawShell();
	void spawnShell(Cannon cannon);
	void updatePos(float dt);
};
