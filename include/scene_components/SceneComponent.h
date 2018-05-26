#ifndef _SCENECOMPONENT_H_
#define _SCENECOMPONENT_H_

#include "graphics/Shader.h"
#include "graphics/ObjContainer.h"

class SceneComponent
{
protected:
    void drawObject(ObjContainer* obj, Shader* shader);

public:
    virtual void onSetup() = 0;
    virtual void onRender(Shader* shader) = 0;
};

#endif