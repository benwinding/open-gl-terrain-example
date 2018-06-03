#include <GL/glew.h>
#include <iostream>
#include "external_files/stb_image.h"
#include "scene/MirrorBox.h"
#include "scene/SceneComponent.h"

MirrorBox::MirrorBox(float scale, glm::vec3 location)
{
    this->scale = scale;
    this->location = location;
    this->onSetup();
}

void MirrorBox::onSetup()
{
    const char* fname = "res/models/cube-simple/cube-simple.obj";
    this->objContainer = new ObjContainer((char*) fname);
    this->shader = new Shader("res/mirrorbox.vert","res/mirrorbox.frag");
    // load textures
    // -------------
    std::vector<std::string> faces
    {
        "res/skybox/right.jpg",
        "res/skybox/left.jpg",
        "res/skybox/top.jpg",
        "res/skybox/bottom.jpg",
        "res/skybox/front.jpg",
        "res/skybox/back.jpg",
    };
    this->cubemapTexture = loadCubemap(faces);
}

void MirrorBox::render(glm::mat4 viewMtx, glm::mat4 projectionMtx)
{
    // Set common uniforms
    this->shader->use();
    shader->setMat4("view", viewMtx);
    shader->setMat4("projection", projectionMtx);

    ObjContainer* obj = this->objContainer;
    glm::mat4 modelM(1.f);
    // Align to top or bottom
    float align = 1; // align bottom = 1, align top = -1
    float objHeight = obj->GetObjSize().y;
    // Move to side
    modelM = glm::scale(modelM, glm::vec3(scale));
    modelM = glm::translate(modelM, -obj->GetOffsetCenter());
    modelM = glm::translate(modelM, glm::vec3(0, align * objHeight/2, 0));
    modelM = glm::translate(modelM, this->location / scale);
    shader->setMat4("model", modelM);
    shader->setVec3("cameraPos", GetCameraPosition(viewMtx));
    // Draw object
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
    this->drawObject(obj, this->shader);
}

void MirrorBox::drawObject(ObjContainer* obj, Shader* shader)
{
    int numShapes = obj->shapes.size();
    for (int i = 0; i < numShapes; ++i)
    {
        Shape currentShape = obj->shapes[i];
        // shader->EnableTexture(currentShape.textureDiffuseId);
        currentShape.DrawShape();
    }
}

// loads a cubemap texture from 6 individual texture faces
// order:
// +X (right)
// -X (left)
// +Y (top)
// -Y (bottom)
// +Z (front) 
// -Z (back)
// -------------------------------------------------------
unsigned int MirrorBox::loadCubemap(std::vector<std::string> faces)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    return textureID;
}

