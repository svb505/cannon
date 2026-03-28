#pragma once
#include <windows.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <random>
#include "shell.h"
#include "gethight.h"

class Target {
private:
	std::vector<float> pos = { 0,0,0 };
	float s = 0.2;
public:
	float getS() { return s; }
	std::vector<float> getCoordinates() { return pos; }
	void draw();
	void initTarget();
	bool checkCollision(float sX, float sY, float sZ, float sStep);
};