#ifndef _OBJINSTANCED_H_
#define _OBJINSTANCED_H_

#include <string>
#include <vector>
#include "glm/glm.hpp"
#include "graphics/ObjContainer.h"
#include "graphics/Shader.h"
#include "scene/SceneComponent.h"

struct InstanceParams {
    glm::vec3 scale;
    glm::vec3 location;
    glm::vec3 rotation;
};

class ObjInstanced : public SceneComponent
{
private:
    const char* fname;
    Shader* shader;
    ObjContainer* objContainer;
    std::vector<InstanceParams*> instanceList;

public:
    ObjInstanced(std::vector<InstanceParams*> instances, std::string fname);
    void onSetup();
    void render(glm::mat4 viewMtx, glm::mat4 projectionMtx);
    void drawObject(ObjContainer* obj, Shader* shader);
};

#endif