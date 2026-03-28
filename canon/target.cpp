#include <windows.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <random>
#include "shell.h"
#include "gethight.h"
#include "target.h"
#include <iostream>

void Target::draw() {
	glPushMatrix();

	glTranslatef(pos[0], pos[1], pos[2]);

	glColor3f(1.0f, 1.0f, 0.0f);
	glBegin(GL_QUADS);

	// front
	glNormal3f(0, 0, 1);
	glVertex3f(-s, -s, s); glVertex3f(s, -s, s); glVertex3f(s, s, s); glVertex3f(-s, s, s);

	// back
	glNormal3f(0, 0, -1);
	glVertex3f(-s, -s, -s); glVertex3f(s, -s, -s); glVertex3f(s, s, -s); glVertex3f(-s, s, -s);

	// left
	glNormal3f(-1, 0, 0);
	glVertex3f(-s, -s, -s); glVertex3f(-s, -s, s); glVertex3f(-s, s, s); glVertex3f(-s, s, -s);

	// right
	glNormal3f(1, 0, 0);
	glVertex3f(s, -s, -s); glVertex3f(s, -s, s); glVertex3f(s, s, s); glVertex3f(s, s, -s);

	// top
	glNormal3f(0, 1, 0);
	glVertex3f(-s, s, -s); glVertex3f(-s, s, s); glVertex3f(s, s, s); glVertex3f(s, s, -s);

	// bottom
	glNormal3f(0, -1, 0);
	glVertex3f(-s, -s, -s); glVertex3f(-s, -s, s); glVertex3f(s, -s, s); glVertex3f(s, -s, -s);

	glEnd();

	glPopMatrix();
}
void Target::initTarget() {
	std::random_device rd;
	static std::mt19937 gen(rd());

	std::uniform_real_distribution<float> distX(-20.0f, 20.0f);
	std::uniform_real_distribution<float> distZ(-20.0f, -90.0f);

	std::uniform_real_distribution<float> distS(0.6f, 0.9f);

	pos[0] = distX(gen);
	pos[2] = distZ(gen);

	pos[1] = getHeight(pos[0], pos[2]);
	s = distS(gen);

}
bool Target::checkCollision(float sX, float sY, float sZ, float sStep) {
	float tgMinX = pos[0] - s;
	float tgMaxX = pos[0] + s;
	float tgMinY = pos[1] - s;
	float tgMaxY = pos[1] + s;
	float tgMinZ = pos[2] - s;
	float tgMaxZ = pos[2] + s;

	float sMinX = sX - sStep;
	float sMaxX = sX + sStep;
	float sMinY = sY - sStep;
	float sMaxY = sY + sStep;
	float sMinZ = sZ - sStep;
	float sMaxZ = sZ + sStep;

	return (tgMinX <= sMaxX && tgMaxX >= sMinX) &&
		(tgMinY <= sMaxY && tgMaxY >= sMinY) &&
		(tgMinZ <= sMaxZ && tgMaxZ >= sMinZ);
}