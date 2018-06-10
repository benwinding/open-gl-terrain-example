#ifndef _PLANT_H_
#define _PLANT_H_

#include "glm/glm.hpp"
#include "graphics/ObjContainer.h"
#include "graphics/Shader.h"
#include "scene/SceneComponent.h"

class Plant : public SceneComponent
{
private:
    float scale;
    glm::vec3 location;
    const char* fname;
    Shader* shader;
    ObjContainer* objContainer;

public:
    Plant(float scale, glm::vec3 location, const char* fname);
    void onSetup();
    void render(glm::mat4 viewMtx, glm::mat4 projectionMtx);
    void drawObject(ObjContainer* obj, Shader* shader);
};

#endif