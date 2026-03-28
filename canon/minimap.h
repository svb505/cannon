#pragma once
#include <vector>

class Target;
class ExplosionEffect;
class SmokeEffect;
class Camera;

class MiniMap {
private:
    float height = 80.0f;
public:
    int step = 3;

    void setHeight(float h) { height = h; }
    float getHeight() { return height; }
    void draw(int ECRANW, int ECRANH, Cannon& cn, Target& trg, std::vector<ExplosionEffect*>& explosions,
        std::vector<SmokeEffect*>& smokes, Camera& camera,float dt);
};


