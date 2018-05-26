#ifndef _WORLDFLOOR_H_
#define _WORLDFLOOR_H_

#include "scene_components/SceneComponent.h"
#include "graphics/Shader.h"

class WorldFloor : SceneComponent
{
    ObjContainer* objContainer;
    float objScale;
    bool alignBottom;

public:
    WorldFloor(float scale, bool alignBottom);
    void onSetup();
    void onRender(Shader* shader);    
};

#endif