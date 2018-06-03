#include <iostream>
#include <stdio.h>
#include <math.h>
#include <string>

#include "graphics/Viewer.h"
#include "glm/gtc/matrix_transform.hpp"
#include "utils/Logger.h"

#define DEG2RAD(x) ((x)*M_PI/180.0) 
#define RAD2DEG(x) ((x)*180.0/M_PI) 

Viewer::Viewer( glm::vec3 eye )
{
    initEye = eye;
    reset();
}

Viewer::~Viewer(){}

/**
 Assumes the current matrix has been calculated (usually in 
 update() or reset())
*/
const glm::mat4 Viewer::getViewMtx() const
{
    return viewMtx;
}

/**
 Resets the view matrix to the value the camera was 
 initialised with. Assumes looking at the origin.
*/
void Viewer::reset()
{
    glm::vec3 at(0.0f, 1.0f, -1.0f);
    glm::vec3 up(0.0f, 1.0f, 0.0f);
    viewMtx = glm::lookAt(initEye, at, up);
}

ObjectViewer::ObjectViewer(glm::vec3 eye) : Viewer(eye)
{
    this->velocity = 0;
}

void ObjectViewer::updateFromPlayer(glm::vec3 location, glm::vec3 direction) 
{
    viewMtx = glm::mat4(0);
    glm::vec3 up(0.0f, 1.0f, 0.0f);
    glm::vec3 target = location + direction;
    viewMtx = glm::lookAt(location, target, up);
}

glm::vec3 ObjectViewer::GetCameraPosition() 
{
    float x = this->viewMtx[3][0];
    float y = this->viewMtx[3][1];
    float z = this->viewMtx[3][2];
    glm::vec3 currentEye = glm::vec3(x,y,z);
    return currentEye;
}

// Overhead Viewer

TopObjectViewer::TopObjectViewer(glm::vec3 eye) : Viewer(eye)
{
    this->height = height;
}

void TopObjectViewer::updateFromPlayer(glm::vec3 location, glm::vec3 direction) 
{
    viewMtx = glm::mat4(0);
    glm::vec3 up = direction;
    glm::vec3 target(location);
    glm::vec3 locationAbove = location + glm::vec3(0,2,0);
    viewMtx = glm::lookAt(locationAbove, target, up);
}

glm::vec3 TopObjectViewer::GetCameraPosition() 
{
    float x = this->viewMtx[3][0];
    float y = this->viewMtx[3][1];
    float z = this->viewMtx[3][2];
    glm::vec3 currentEye = glm::vec3(x,y,z);
    return currentEye;
}

