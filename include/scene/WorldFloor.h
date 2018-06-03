#ifndef _WORLDFLOOR_H_
#define _WORLDFLOOR_H_

#include "graphics/Shader.h"
#include "graphics/ObjContainer.h"

enum alignment {
    ALIGN_BOTTOM,
    ALIGN_TOP
};

class WorldFloor
{
    ObjContainer* objContainer;
    float objScale;
    int align;
    void drawObject(ObjContainer* obj, Shader* shader);

public:
    WorldFloor(float scale, int alignment);
    void onSetup();
    void onRender(Shader* shader);    
};

#endif