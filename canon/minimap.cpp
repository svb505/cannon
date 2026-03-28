#include <windows.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <GL/glu.h>
#include "environnement.h"
#include "effects.h"
#include "camera.h"
#include "shell.h"
#include "minimap.h"
#include "target.h"

void MiniMap::draw(int ECRANW, int ECRANH, Cannon& cn, Target& trg, std::vector<ExplosionEffect*>& explosions,
    std::vector<SmokeEffect*>& smokes, Camera& camera, float dt) {

    float size = height * 0.6f;
    int miniW = ECRANW / 4;
    int miniH = ECRANH / 4;
    auto coords = trg.getCoordinates();

    glViewport(ECRANW - miniW, 30, miniW, miniH);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-size, size, -size, size, -500, 500);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(
        coords[0] - 8.0f,
        coords[1] + 5.0f,
        coords[2] + 8.0f,

        coords[0],
        coords[1],
        coords[2],

        0, 1, 0
    );

    glClear(GL_DEPTH_BUFFER_BIT);

    drawGround(cam.x,cam.z);

    trg.draw();

    for (auto it = explosions.begin(); it != explosions.end(); ++it) (*it)->Draw();
    for (auto it = smokes.begin(); it != smokes.end(); ++it) (*it)->Draw();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, miniW, 0, miniH, -1, 1);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glColor3f(1.0f, 1.0f, 1.0f);
    glLineWidth(2.0f);

    glBegin(GL_LINE_LOOP);
    glVertex2f(0, 0);
    glVertex2f(miniW, 0);
    glVertex2f(miniW, miniH);
    glVertex2f(0, miniH);
    glEnd();

    glViewport(0, 0, ECRANW, ECRANH);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(camera.fov, (float)ECRANW / ECRANH, camera.nearPlane, camera.farPlane);
}

