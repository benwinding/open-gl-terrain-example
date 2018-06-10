#ifndef _FOUNTAIN_H_
#define _FOUNTAIN_H_

#include "glm/glm.hpp"
#include "graphics/ObjContainer.h"
#include "graphics/Shader.h"
#include "scene/SceneComponent.h"
#include "utils/Random.h"

struct FountainParticle
{
private:
    int minLife = 30;
    int maxLife = 200;
    float minSpeed = 1;
    float maxSpeed = 10;

public:
    int life = 10;
    int age = 0;
    float speed = 0;
    float temperature = 0;
    float initx = 0;
    float initz = 0;
    float variance = 0;

    void respawn() {
        srand(getTime());
        this->life = randomFloat(this->minLife, this->maxLife);
        this->age = randomFloat(0, this->life);
        this->speed = randomFloat(this->minSpeed, this->maxSpeed);
        this->temperature = randomFloat(0, 1);
        this->initx = randomFloat(-1, 1);
        this->initz = randomFloat(-1, 1);
        this->variance = randomFloat(-1, 1);
    }

    int maxDist = this->maxLife * this->maxSpeed;
};

class Fountain : public SceneComponent
{
private:
    float scale;
    glm::vec3 location;
    Shader* shader;
    unsigned int cubeVAO, cubeVBO;
    FountainParticle* particles;
    int particleCount;

    float fountainHeight = 1;
    float fountainWidth = 1;

public:
    Fountain(float fountainHeight, float fountainWidth, int particleCount, glm::vec3 location);
    void onSetup();
    void render(glm::mat4 viewMtx, glm::mat4 projectionMtx);
    void drawObject();
};

#endif