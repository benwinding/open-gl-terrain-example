#ifndef _MIRROR_BOX_H_
#define _MIRROR_BOX_H_

#include "glm/glm.hpp"
#include "graphics/ObjContainer.h"
#include "graphics/Shader.h"
#include "scene/SceneComponent.h"

// Repsonsible For Tracking the players movements
class MirrorBox : public SceneComponent
{
private:
    float scale;
    glm::vec3 location;
    Shader* shader;
    ObjContainer* objContainer;
    unsigned int cubemapTexture;
    unsigned int loadCubemap(std::vector<std::string> faces);

public:
    MirrorBox(float scale, glm::vec3 location);
    void onSetup();
    void render(glm::mat4 viewMtx, glm::mat4 projectionMtx);
    void drawObject(ObjContainer* obj, Shader* shader);
};

#endif