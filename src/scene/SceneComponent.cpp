#include "scene/SceneComponent.h"

void SceneComponent::setCamPosition(glm::vec3 camPos)
{
    this->camPos = camPos;
}

glm::vec3 SceneComponent::getCamPosition()
{
    return this->camPos;
}