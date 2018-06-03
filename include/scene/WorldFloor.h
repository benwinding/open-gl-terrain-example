#ifndef _WORLDFLOOR_H_
#define _WORLDFLOOR_H_

#include "glm/glm.hpp"

#include "graphics/Shader.h"
#include "graphics/ObjContainer.h"
#include "scene/SceneComponent.h"

enum alignment 
{
    ALIGN_BOTTOM,
    ALIGN_TOP
};

class WorldFloor : public SceneComponent
{
private:
    Shader* shader;
    ObjContainer* objContainer;
    float objScale;
    int align;
    void drawObject(ObjContainer* obj, Shader* shader);

public:
    WorldFloor(float scale, int alignment);
    void render(glm::mat4 viewMtx, glm::mat4 projectionMtx);
    void onSetup();
};

#endif