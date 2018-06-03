#ifndef _SCENE_COMPONENT_H_
#define _SCENE_COMPONENT_H_

#include "glm/glm.hpp"

// Repsonsible For Tracking the players movements
class SceneComponent 
{
public:
    virtual void onSetup() = 0;
    virtual void onRender() = 0;
    virtual void setViewProjection(glm::mat4 viewMtx, glm::mat4 projectionMtx) = 0;
};

#endif