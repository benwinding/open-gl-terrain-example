#include <vector>
#include <string>
#include <cmath>
#include "glm/glm.hpp"

#include "SceneCalculations.h"
#include "ObjContainer.h"
#include "Logger.h"

glm::vec3 CalculateBoundingBoxTotal(std::vector<ObjContainer*> objList) 
{
    glm::vec3 totalExtents(0);
    for (int i = 0; i < objList.size(); ++i)
    {
        glm::vec3 size = objList[i]->GetObjSize();
        totalExtents.x += size.x;
        if(size.y > totalExtents.y)        
            totalExtents.y = size.y;
        if(size.z > totalExtents.z)        
            totalExtents.z = size.z;
    }
    return totalExtents;
}

float CalculateTotalScale(std::vector<ObjContainer*> objList) 
{
    glm::vec3 totalExtents = CalculateBoundingBoxTotal(objList);
    float length = glm::length(totalExtents);
    float MAX_LENGTH = 4.f;    
    float scale = MAX_LENGTH / length;
    return scale;
}

glm::vec3 GetRotatingPosition() 
{
    static double startTime = glfwGetTime();
    double t = (startTime - glfwGetTime())*1;
    float r = 6.f;
    float x = r*sin(t);
    float y = 0.0f;
    float z = r*cos(t);
    return glm::vec3(x,y,z);
}

