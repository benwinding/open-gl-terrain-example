#include "scene/ObjSingle.h"

ObjSingle::ObjSingle(float scale, glm::vec3 location, std::string fname)
{
    this->scale = scale;
    this->location = location;
    this->fname = fname.c_str();
    this->onSetup();
}

void ObjSingle::onSetup() {
    this->objContainer = new ObjContainer((char*) this->fname);
    this->shader = new Shader("res/objsingle.vert","res/objsingle.frag");
}

void ObjSingle::render(glm::mat4 viewMtx, glm::mat4 projectionMtx) {
    // Set common uniforms
    this->shader->use();
    shader->setMat4("view", viewMtx);
    shader->setMat4("projection", projectionMtx);

    ObjContainer* obj = this->objContainer;
    glm::mat4 modelM(1.f);
    // Align to top or bottom
    float align = 1; // align bottom = 1, align top = -1
    float objHeight = obj->GetObjSize().y;
    float normalizeScale = scale / objHeight;
    // Move to side
    modelM = glm::scale(modelM, glm::vec3(normalizeScale));
    modelM = glm::translate(modelM, -obj->GetOffsetCenter());
    modelM = glm::translate(modelM, glm::vec3(0, align * objHeight/2, 0));
    modelM = glm::translate(modelM, this->location / normalizeScale);
    shader->setMat4("model", modelM);
    // Draw object
    this->drawObject(obj, this->shader);
}

void ObjSingle::drawObject(ObjContainer* obj, Shader* shader) 
{
    int numShapes = obj->shapes.size();
    for (int i = 0; i < numShapes; ++i)
    {
        Shape currentShape = obj->shapes[i];
        shader->setInt("material.texture", 0);
        shader->EnableTexture(currentShape.textureDiffuseId);
        currentShape.DrawShape();
    }
}

