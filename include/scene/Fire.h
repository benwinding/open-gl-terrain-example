#ifndef _FIRE_H_
#define _FIRE_H_

#include "glm/glm.hpp"
#include "graphics/ObjContainer.h"
#include "graphics/Shader.h"
#include "scene/SceneComponent.h"

struct FireParticle
{
    int life = 10;
    int age = 0;
    float speed = 0;
    float temperature = 0;
    float initx = 0;
    float initz = 0;
    float variance = 0;
};

class Fire : public SceneComponent
{
private:
    float scale;
    glm::vec3 location;
    Shader* shader;
    unsigned int cubeVAO, cubeVBO;
    FireParticle* particles;
    int particleCount;
    float fireHeight = 1;
    float fireWidth = 1;

public:
    Fire(float fireHeight, float fireWidth, int particleCount, glm::vec3 location);
    void onSetup();
    void render(glm::mat4 viewMtx, glm::mat4 projectionMtx);
    void drawObject();
};

#endif