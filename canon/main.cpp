#include <windows.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include <format>
#include <AL/al.h>
#include <AL/alc.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "camera.h"
#include "environnement.h"
#include "canon.h"
#include "text.h"
#include "shell.h"
#include "sounds.h"
#include "target.h"
#include "effects.h"
#include "GUI.h"
#include "lightning.h"
#include "minimap.h"

#define WW 1500
#define WH 900

Camera camera;
Cannon cannon;
Shells shell;
Sound sound;
Target trg;
GUI gui;
Light light;
MiniMap mnm;

int score = 0;
int level = 1;

void fpsCount(double& deltaTime, double& lastTime, float& fpsTimer,int& frames,float& fps) {
    double currentTime = glfwGetTime();
    deltaTime = currentTime - lastTime;
    lastTime = currentTime;

    fpsTimer += deltaTime;
    frames++;

    if (fpsTimer >= 1.0f) {
        fps = frames / fpsTimer;
        frames = 0;
        fpsTimer = 0.0f;
    }
}
void processInput(GLFWwindow* window,float dt){
    float step = 15.0f;
    float camStep = 15.0f;

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        cannon.cannonPitch += step * dt;
        alSourceStop(sound.turretSource);
        alSourcePlay(sound.turretSource);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        cannon.cannonYaw += step * dt;
        alSourceStop(sound.turretSource);
        alSourcePlay(sound.turretSource);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        cannon.cannonPitch -= step * dt;
        alSourceStop(sound.turretSource);
        alSourcePlay(sound.turretSource);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        cannon.cannonYaw -= step * dt;
        alSourceStop(sound.turretSource);
        alSourcePlay(sound.turretSource);
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && cannon.reload <= 0.0f) {
        shell.spawnShell(cannon);
        cannon.reload = cannon.reloadTime;
        sound.setSourcePosition(sound.shootSource, cannon.baseWidth, cannon.baseHeight, cannon.baseDepth);
        alSourcePlay(sound.shootSource);
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) cam.pitch += camStep * 2 * dt;
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) cam.pitch -= camStep * 2 * dt;
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) cam.yaw += camStep * 2 * dt;
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) cam.yaw -= camStep * 2 * dt;
    if (glfwGetKey(window, GLFW_KEY_KP_8) == GLFW_PRESS) cam.y += camStep * dt;
    if (glfwGetKey(window, GLFW_KEY_KP_2) == GLFW_PRESS) cam.y -= camStep * dt;
    if (glfwGetKey(window, GLFW_KEY_KP_4) == GLFW_PRESS) cam.x -= camStep * dt;
    if (glfwGetKey(window, GLFW_KEY_KP_6) == GLFW_PRESS) cam.x += camStep * dt;
    if (glfwGetKey(window, GLFW_KEY_KP_7) == GLFW_PRESS) cam.z -= camStep * dt;
    if (glfwGetKey(window, GLFW_KEY_KP_9) == GLFW_PRESS) cam.z += camStep * dt;
}
int main(){
    sound.setupBuffers();
    sound.createSources();

    if (!glfwInit()){
        std::cout << "Failed to initialize GLFW\n";
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(WW, WH, "Cannon", NULL, NULL);
    if (!window){
        std::cout << "Failed to create window\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    BuildFont();
    glViewport(0, 0, WW, WH);

    glEnable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glFrustum(camera.left, camera.right, camera.bottom, camera.top, camera.nearPlane, camera.farPlane);

    glMatrixMode(GL_MODELVIEW);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330"); 

    ImGui::StyleColorsDark();

    double lastTime = glfwGetTime();
    double deltaTime = 0.0;
    float fpsTimer = 0.0f;
    int frames = 0;
    float fps = 0.0f;

    trg.initTarget();
    std::vector<ExplosionEffect*> explosions;
    std::vector<SmokeEffect*> smokes;

    light.initLighting();

    while (!glfwWindowShouldClose(window)){
        if (!light.enabled) {
            glDisable(GL_LIGHTING);
            glDisable(GL_LIGHT0);
            glDisable(GL_COLOR_MATERIAL);
        }
        else {
            glEnable(GL_LIGHTING);
            glEnable(GL_LIGHT0);
            glEnable(GL_COLOR_MATERIAL);
        }

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        gui.drawGui(score,cannon.reload,fps, cannon,light,trg);

        ImGui::Render();

        fpsCount(deltaTime,lastTime,fpsTimer,frames,fps);
       
        float radYaw = cam.yaw * 3.14159265f / 180.0f;
        float radPitch = cam.pitch * 3.14159265f / 180.0f;
        float fx = cos(radPitch) * sin(radYaw);
        float fy = sin(radPitch);
        float fz = -cos(radPitch) * cos(radYaw);

        sound.setListener(cam.x, cam.y, cam.z, fx, fy, fz);

        if (cannon.reload > 0.0f) cannon.reload -= deltaTime;

        processInput(window,deltaTime);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glPushMatrix();
        glLoadIdentity();
        glRotatef(-cam.pitch, 1, 0, 0);
        glRotatef(-cam.yaw, 0, 1, 0);

        drawSky();

        glPopMatrix();

        glLoadIdentity();
        glRotatef(-cam.pitch, 1, 0, 0);
        glRotatef(-cam.yaw, 0, 1, 0);
        glTranslatef(-cam.x, -cam.y, -cam.z + 3.0f);

        drawGround(cam.x, cam.z);
        drawGrid(cam.x, cam.z);
        if (gridTextEnadled) drawGridText(cam.x, cam.z);

        trg.draw();

        glPushMatrix();
        glTranslatef(0, 0, -10);
        cannon.drawCannon();
        glPopMatrix();

        if (level == 1) cannon.drawTrajectory(0.0f, 0.5f, -13.0f,cannon.cannonYaw,cannon.cannonPitch,
            shellSpeed,shell.pos.gravity);

        shell.updatePos(deltaTime);

        for (auto& s : shell.shells) {
            glPushMatrix();
            glTranslatef(s.x,s.y,s.z);

            shell.drawShell();
          
            glPopMatrix();

            if (s.y <= 0 && !s.alive) {
                explosions.push_back(new ExplosionEffect(s.x, s.y, s.z));
                smokes.push_back(new SmokeEffect(s.x, s.y, s.z));
                
                sound.setSourcePosition(sound.explosionSource, s.x, s.y, s.z);
                alSourcePlay(sound.explosionSource);

                s.alive = true;
            }
            if (trg.checkCollision(s.x,s.y,s.z,1.0f)) {
                score++;
                trg.initTarget();
            }
        }
        for (auto it = explosions.begin(); it != explosions.end();) {
            (*it)->Update(deltaTime);
            (*it)->Draw();

            if ((*it)->IsFinished()) {
                delete* it;
                it = explosions.erase(it);
            }
            else ++it;
        }
        for (auto it = smokes.begin(); it != smokes.end(); ) {
            SmokeEffect* smoke = *it;

            bool alive = false;
            for (auto& p : smoke->getCoordinates()) {
                if (p.y <= 5.0f) {
                    alive = true;
                    break;
                }
            }
            if (!alive) {
                delete smoke;
                it = smokes.erase(it);
            }
            else {
                smoke->Update(deltaTime);
                smoke->Draw();
                ++it;
            }

        }
        for (auto it = shell.shells.begin(); it != shell.shells.end();) {
            if ((*it).y <= 0.0f) it = shell.shells.erase(it);
            else ++it;
        }

        mnm.draw(WW, WH, cannon, trg, explosions, smokes, camera, deltaTime);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}