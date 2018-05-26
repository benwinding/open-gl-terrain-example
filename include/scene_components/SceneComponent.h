#ifndef _SCENECOMPONENT_H_
#define _SCENECOMPONENT_H_

class SceneComponent
{
public:
    virtual void onSetup() = 0;
    virtual void onRender() = 0;
};

#endif