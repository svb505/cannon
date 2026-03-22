#pragma once
#include <windows.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include "gethight.h"

class Cannon {
public:
    // ===== Параметры пушки =====
    float baseWidth = 2.0f;   // ширина корпуса
    float baseHeight = getHeight(2.0f / 2, 3.0f / 2);   // высота корпуса
    float baseDepth = 3.0f;   // глубина корпуса

    float barrelRadius = 0.2f; // радиус ствола
    float barrelLength = 3.0f; // длина ствола
    float barrelOffsetY = 0.5f; // сдвиг ствола вверх от основания
    float barrelOffsetZ = 1.0f; // сдвиг ствола вперёд перед наклоном

    float cannonYaw = 180.0f;
    float cannonPitch = 20.0f;
    float reloadTime = 3.0f;
    float reload = 0.0f;

    void drawBox(float w, float h, float d) {

        float x = w / 2;
        float y = h;
        float z = d / 2;

        glBegin(GL_QUADS);

        glNormal3f(0, 0, 1);
        glVertex3f(-x, -y, z); glVertex3f(x, -y, z); glVertex3f(x, y, z); glVertex3f(-x, y, z);

        glNormal3f(0, 0, -1);
        glVertex3f(-x, -y, -z); glVertex3f(-x, y, -z); glVertex3f(x, y, -z); glVertex3f(x, -y, -z);

        glNormal3f(-1, 0, 0);
        glVertex3f(-x, -y, -z); glVertex3f(-x, -y, z); glVertex3f(-x, y, z); glVertex3f(-x, y, -z);

        glNormal3f(1, 0, 0);
        glVertex3f(x, -y, -z); glVertex3f(x, y, -z); glVertex3f(x, y, z); glVertex3f(x, -y, z);

        glNormal3f(0, 1, 0);
        glVertex3f(-x, y, -z); glVertex3f(-x, y, z); glVertex3f(x, y, z); glVertex3f(x, y, -z);

        glNormal3f(0, -1, 0);
        glVertex3f(-x, -y, -z); glVertex3f(x, -y, -z); glVertex3f(x, -y, z); glVertex3f(-x, -y, z);

        glEnd();
    }
    void drawCylinderCap(float radius, float z, int slices, bool front) {
        glBegin(GL_TRIANGLE_FAN);

        glNormal3f(0, 0, front ? -1.0f : 1.0f);
        glVertex3f(0, 0, z);

        for (int i = 0; i <= slices; i++) {
            float a = 2.0f * 3.1415926f * i / slices;
            float x = cos(a) * radius;
            float y = sin(a) * radius;
            glVertex3f(x, y, z);
        }

        glEnd();
    }
    void drawBetterCylinder(float radius, float length, int slices) {
        // боковая поверхность
        glBegin(GL_QUAD_STRIP);
        for (int i = 0; i <= slices; i++) {
            float a = 2.0f * 3.1415926f * i / slices;

            float x = cos(a);
            float y = sin(a);

            glNormal3f(x, y, 0);
            glVertex3f(x * radius, y * radius, 0);
            glVertex3f(x * radius, y * radius, length);
        }
        glEnd();

        // крышки
        drawCylinderCap(radius, 0.0f, slices, true);
        drawCylinderCap(radius, length, slices, false);
    }

    void drawCannon() {
        // ===== Основание (гусеничная платформа) =====
        glPushMatrix();
        glColor3f(0.25f, 0.25f, 0.25f);
        drawBox(baseWidth, baseHeight, baseDepth);
        glPopMatrix();

        // ===== Башня =====
        glPushMatrix();
        glTranslatef(0.0f, baseHeight * 0.5f, 0.0f);

        glRotatef(cannonYaw, 0.0f, 1.0f, 0.0f);

        glColor3f(0.35f, 0.35f, 0.35f);
        drawBox(baseWidth * 0.7f, baseHeight * 0.6f, baseDepth * 0.7f);

        // ===== Ствол =====
        glPushMatrix();

        glTranslatef(0.0f, barrelOffsetY, barrelOffsetZ);
        glRotatef(-cannonPitch, 1.0f, 0.0f, 0.0f);

        // Основание ствола (толще)
        glColor3f(0.3f, 0.3f, 0.3f);
        drawBetterCylinder(barrelRadius * 1.5f, barrelLength * 0.2f, 32);

        // Основной ствол
        glTranslatef(0.0f, 0.0f, barrelLength * 0.2f);
        glColor3f(0.5f, 0.5f, 0.5f);
        drawBetterCylinder(barrelRadius, barrelLength * 0.7f, 32);

        // Дульный тормоз
        glTranslatef(0.0f, 0.0f, barrelLength * 0.7f);
        glColor3f(0.2f, 0.2f, 0.2f);
        drawBetterCylinder(barrelRadius * 1.2f, barrelLength * 0.1f, 32);

        glPopMatrix(); // ствол

        glPopMatrix(); // башня
    }
    void drawTrajectory(float startX, float startY, float startZ,float yaw, float pitch,
        float speed, float gravity){
        float yawRad = yaw * 3.1415926f / 180.0f;
        float pitchRad = pitch * 3.1415926f / 180.0f;

        float vx = cos(pitchRad) * sin(yawRad) * speed;
        float vy = (sin(pitchRad) * speed);
        float vz = cos(pitchRad) * cos(yawRad) * speed;

        glColor3f(1, 0, 0);

        glBegin(GL_LINE_STRIP);

        float timeStep = 0.05f;

        for (float t = 0; t < 5.0f; t += timeStep){
            float x = startX + vx * t;
            float y = startY + vy * t - 0.5f * gravity * t * t;
            float z = startZ + vz * t + 5.0f;

            if (y < 0.0f) break; 

            glVertex3f(x, y, z);
        }

        glEnd();
    }
};



