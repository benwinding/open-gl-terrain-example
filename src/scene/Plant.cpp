#include "scene/Plant.h"

Plant::Plant(float scale, glm::vec3 location)
{
    this->scale = scale;
    this->location = location;
    this->onSetup();
}

void Plant::onSetup() {
    const char* fname = "res/models/Barrel/Barrel02.obj";
    this->objContainer = new ObjContainer((char*) fname);
    this->shader = new Shader("res/plant.vert","res/plant.frag");
}

void Plant::render(glm::mat4 viewMtx, glm::mat4 projectionMtx) {
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
    // Draw object
    this->drawObject(obj, this->shader);
}

void Plant::drawObject(ObjContainer* obj, Shader* shader) 
{
    int numShapes = obj->shapes.size();
    for (int i = 0; i < numShapes; ++i)
    {
        Shape currentShape = obj->shapes[i];
        shader->setVec3("diffuse", currentShape.material.diffuse);
        currentShape.DrawShape();
    }
}

