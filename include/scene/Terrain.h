#ifndef _TERRAIN_H_
#define _TERRAIN_H_

#include <vector>
#include "glm/glm.hpp"
#include "graphics/ObjContainer.h"
#include "graphics/Shader.h"
#include "graphics/Viewer.h"
#include "scene/SceneComponent.h"

struct shape_terrain
{
    unsigned int VAO;   
    unsigned int vCount;   
};

// Repsonsible For Tracking the players movements
class Terrain : public SceneComponent
{
private:
    glm::vec3 terrainLocation;
    glm::vec3 terrainSize;
    glm::ivec3 gridCount; 

    Shader* shader;
    std::vector<shape_terrain> shapes;
    void AddVertex(std::vector<float> *verts, float* terrain, int x, int z);
    void finishShape(std::vector<float> verts);
public:
    Terrain(glm::vec3 terrainLocation, glm::vec3 terrainSize, glm::ivec3 gridCount);
    void onSetup();
    void render(glm::mat4 viewMtx, glm::mat4 projectionMtx);
};

#endif