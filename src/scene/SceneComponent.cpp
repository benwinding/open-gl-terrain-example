#include "scene/SceneComponent.h"

glm::vec3 SceneComponent::GetCameraPosition(glm::mat4 viewMtx) 
{
    float x = viewMtx[3][0];
    float y = viewMtx[3][1];
    float z = viewMtx[3][2];
    glm::vec3 currentEye = glm::vec3(x,y,z);
    return currentEye;
}

void SceneComponent::setCamPosition(glm::vec3 camPos)
{
    this->camPos = camPos;
}