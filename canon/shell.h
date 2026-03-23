#pragma once
#include "canon.h"
#include <cmath>
#include <vector>

float shellSpeed = 25.0f;

struct Vec3 {
	float x, y, z;
};

Vec3 computeImpact(Vec3 p0, float v, float pitch, float yaw) {
	const float g = 9.81f;

	float pitchRad = pitch * 3.14159265f / 180.0f;
	float yawRad = yaw * 3.14159265f / 180.0f;

	float vx = cos(pitchRad) * sin(yawRad) * shellSpeed;
	float vy = (sin(pitchRad) * shellSpeed);
	float vz = cos(pitchRad) * cos(yawRad) * shellSpeed;

	float D = vy * vy + 2.0f * g * p0.y;
	if (D < 0.0f)
		return { NAN, NAN, NAN };

	float t = (vy + sqrt(D)) / g;

	Vec3 hit;
	hit.x = p0.x + vx * t;
	hit.y = 0.0f;
	hit.z = p0.z + vz * t;

	return hit;
}
struct Position {
	float x0, y0, z0;
	float x, y, z;
	float vx, vy, vz;
	float gravity = 9.8f;
	float t;
	bool alive = false;
};

class Shells {
public:
	float s = 0.1f;

	Position pos;
	std::vector<Position> shells;

	void drawShell() {

		glColor3f(192.0f / 255.0f, 192.0f / 255.0f, 192.0f / 255.0f);
		glBegin(GL_QUADS);

		glNormal3f(0, 0, 1);
		glVertex3f(-s, -s, s); glVertex3f(s, -s, s); glVertex3f(s, s, s); glVertex3f(-s, s, s);

		glNormal3f(0, 0, -1);
		glVertex3f(-s, -s, -s); glVertex3f(s, -s, -s); glVertex3f(s, s, -s); glVertex3f(-s, s, -s);

		glNormal3f(-1, 0, 0);
		glVertex3f(-s, -s, -s); glVertex3f(-s, -s, s); glVertex3f(-s, s, s); glVertex3f(-s, s, -s);

		glNormal3f(1, 0, 0);
		glVertex3f(s, -s, -s); glVertex3f(s, -s, s); glVertex3f(s, s, s); glVertex3f(s, s, -s);

		glNormal3f(0, 1, 0);
		glVertex3f(-s, s, -s); glVertex3f(-s, s, s); glVertex3f(s, s, s); glVertex3f(s, s, -s);

		glNormal3f(0, -1, 0);
		glVertex3f(-s, -s, -s); glVertex3f(-s, -s, s); glVertex3f(s, -s, s); glVertex3f(s, -s, -s);

		glEnd();
	}
	void spawnShell(Cannon cannon) {
		float yawRad = cannon.cannonYaw * 3.1415926f / 180.0f;
		float pitchRad = cannon.cannonPitch * 3.1415926f / 180.0f;
		float L = cannon.barrelLength;

		float xTop = cannon.baseWidth + L * cos(pitchRad) * sin(yawRad);
		float zTop = cannon.baseDepth - L * cos(pitchRad) * cos(yawRad);
		float yTop = cannon.baseHeight + L * sin(pitchRad);

		Position pos;
		pos.x0 = 0.0f;
		pos.y0 = yTop;
		pos.z0 = -5.0f;

		pos.vx = cos(pitchRad) * sin(yawRad) * shellSpeed;
		pos.vy = (sin(pitchRad) * shellSpeed);
		pos.vz = cos(pitchRad) * cos(yawRad) * shellSpeed;

		pos.gravity = 9.8f;
		pos.t = 0.0f;

		pos.x = pos.x0;
		pos.y = pos.y0;
		pos.z = pos.z0;

		shells.push_back(pos);
	}
	void updatePos(float dt) {
		for (auto& s : shells) {
			s.t += dt;

			s.x = s.x0 + s.vx * s.t;
			s.y = s.y0 + s.vy * s.t - 0.5f * s.gravity * s.t * s.t;
			s.z = s.z0 + s.vz * s.t;
		}
	}
};
