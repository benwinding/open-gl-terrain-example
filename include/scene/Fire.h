#ifndef _FIRE_H_
#define _FIRE_H_

#include "glm/glm.hpp"
#include "graphics/ObjContainer.h"
#include "graphics/Shader.h"
#include "scene/SceneComponent.h"

class Fire : public SceneComponent
{
private:
    float scale;
    glm::vec3 location;
    Shader* shader;
    unsigned int cubeVAO, cubeVBO;

public:
    Fire(float scale, glm::vec3 location);
    void onSetup();
    void render(glm::mat4 viewMtx, glm::mat4 projectionMtx);
    void drawObject();
};

#endif