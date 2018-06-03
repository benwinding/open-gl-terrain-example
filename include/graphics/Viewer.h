
/**
 Some viewer classes representing cameras that fixate on an object.
 All viewers inherit from a base Viewer class.
 See model-view.cpp for example usage.
 Anthony Dick
*/

#ifndef _VIEWER_H_
#define _VIEWER_H_

#include "graphics/InputState.h"
#include "glm/glm.hpp"

/**
 The base Viewer class stores a current view transformation.
 It is initialised by providing an initial camera position.
 Assumes we are always looking at the origin.
 It is updated in response to user input or it can be reset.
 Method orthogonaliseViewMtx() can be used to correct drift if needed.
*/
class Viewer 
{
protected:
    glm::mat4 viewMtx;
    glm::vec3 initEye;
    glm::vec3 initAt;
    glm::vec3 initUp;

public:
    Viewer( glm::vec3 eye );
    virtual ~Viewer();

    const glm::mat4 getViewMtx() const;
    //    void orthogonaliseViewMtx();
    void reset();

    virtual void updateFromPlayer(glm::vec3 location, glm::vec3 direction) = 0;
    virtual glm::vec3 GetCameraPosition() = 0;
};


class ObjectViewer : public Viewer
{
private:
    float velocity;
public:
    ObjectViewer( glm::vec3 eye );
    void updateFromPlayer(glm::vec3 location, glm::vec3 direction);
    glm::vec3 GetCameraPosition();
};


class TopObjectViewer : public Viewer
{
private:
    float height;
public:
    TopObjectViewer( glm::vec3 eye );
    void updateFromPlayer(glm::vec3 location, glm::vec3 direction);
    glm::vec3 GetCameraPosition();
};







#endif // VIEWER_H
