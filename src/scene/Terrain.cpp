#include <iostream>
#include <cmath>
#include <vector>
#include <GL/glew.h>
#include "glm/glm.hpp"
#include "external_files/stb_image.h"
#include "utils/Random.h"
#include "utils/Logger.h"
#include "scene/Terrain.h"
#include "scene/SceneComponent.h"

Terrain::Terrain(glm::vec3 terrainLocation, glm::vec3 terrainSize, glm::ivec3 gridCount)
{
    this->terrainLocation = terrainLocation;
    this->terrainSize = terrainSize;
    this->gridCount = 3*gridCount;
    this->gridCount.x *= 4;
    this->onSetup();
}

void Terrain::AddVertex(std::vector<float>* verts, float* terrain, int x, int z)
{
    float y = terrain[z*gridCount.z + x];
    glm::vec3 basicPos = glm::vec3(x, y, z);
    float scaleNormalize = glm::max(gridCount.x, gridCount.z);
    glm::vec3 normalizedPos = basicPos * (1 / scaleNormalize);
    glm::vec3 scaledPos = normalizedPos * terrainSize;
    scaledPos.y = basicPos.y;
    glm::vec3 finalPos = scaledPos + terrainLocation;
    verts->push_back(finalPos.x);
    verts->push_back(finalPos.y);
    verts->push_back(finalPos.z);
}

void Terrain::onSetup()
{
    float* terrain = new float[gridCount.z * gridCount.x];
    float zoff = 0;

    float waveFreqX = 0.3;
    float waveFreqZ = 0.3;

    for (int z = 0; z < gridCount.z; z++) 
    {
        for (int x = 0; x < gridCount.x; x++) 
        {
            int index = z * gridCount.z + x;
            terrain[index] = terrainSize.y * Random::wave(x*waveFreqX, z*waveFreqZ);
        }
    }


    for (int z = 0; z < gridCount.z-1; z++)
    {
        std::vector<float> verts;
        for (int x = 0; x < gridCount.x; x++) 
        {
            AddVertex(&verts, terrain, x, z + 0);
            AddVertex(&verts, terrain, x, z + 1);
            AddVertex(&verts, terrain, x + 1, z);
            // Print("xyz=", glm::vec3(verts.at(0), verts.at(1), verts.at(2)));
        }
        finishShape(verts);
    }

    this->shader = new Shader("res/debug_inspect.vert","res/debug_inspect.frag");
}

void Terrain::finishShape(std::vector<float> verts) {
    unsigned int vertsVAO;
    unsigned int vertsVBO;
    glGenVertexArrays(1, &vertsVAO);
    glGenBuffers(1, &vertsVBO);
    glBindVertexArray(vertsVAO);
    glBindBuffer(GL_ARRAY_BUFFER, vertsVBO);
    glBufferData(GL_ARRAY_BUFFER, verts.size(), &verts[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glBindVertexArray(0);
    shape_terrain shape;
    shape.VAO = vertsVAO;
    shape.vCount = verts.size();
    this->shapes.push_back(shape);
}

void Terrain::render(glm::mat4 viewMtx, glm::mat4 projectionMtx)
{
    // Set common uniforms
    this->shader->use();
    shader->setMat4("view", viewMtx);
    shader->setMat4("projection", projectionMtx);

    // Align to top or bottom
    float align = 1; // align bottom = 1, align top = -1
    float objHeight = 1; //obj->GetObjSizeverts).y;
    // Move to side
    glm::mat4 modelM(1.f);
    shader->setMat4("model", modelM);
    shader->setVec3("diffuse", glm::vec3(0,1,0));

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    for (int i = 0; i < this->shapes.size(); ++i)
    {
        shape_terrain shape = this->shapes.at(i);
        int vao = shape.VAO;
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, shape.vCount );
    }
    shader->setVec3("diffuse", glm::vec3(0.1));
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    for (int i = 0; i < this->shapes.size(); ++i)
    {
        shape_terrain shape = this->shapes.at(i);
        int vao = shape.VAO;
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, shape.vCount );
    }
    glBindVertexArray(0);
}
