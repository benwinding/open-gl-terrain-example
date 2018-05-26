#ifndef _WORLDFLOOR_H_
#define _WORLDFLOOR_H_

#include "scene_components/SceneComponent.h"
#include "graphics/Shader.h"

class WorldFloor : SceneComponent
{
    ObjContainer* objContainer;
    float objScale;

public:
    void onSetup();
    void onRender(Shader* shader);    

};

#endif