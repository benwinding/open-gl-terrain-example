#ifndef _WORLDFLOOR_H_
#define _WORLDFLOOR_H_

#include "scene_components/SceneComponent.h"
#include "graphics/Shader.h"

enum alignment {
    ALIGN_BOTTOM,
    ALIGN_TOP
};

class WorldFloor : SceneComponent
{
    ObjContainer* objContainer;
    float objScale;
    int align;

public:
    WorldFloor(float scale, int alignment);
    void onSetup();
    void onRender(Shader* shader);    
};

#endif