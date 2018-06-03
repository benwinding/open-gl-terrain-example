#include "scene/WorldFloor.h"

WorldFloor::WorldFloor(float scale, int align) {
    this->objScale = scale;
    this->align = align;
}

void WorldFloor::setViewProjection(glm::mat4 viewMtx, glm::mat4 projectionMtx) {
    // Set common uniforms
    shader->setMat4("view", viewMtx);
    shader->setMat4("projection", projectionMtx);
}

void WorldFloor::onSetup() {
    const char* fname = "res/models/cube-simple/cube-simple.obj";
    // const char* fname = "res/models/Barrel/Barrel02.obj";
    this->objContainer = new ObjContainer((char*) fname);
    this->shader = new Shader("res/debug_inspect.vert","res/debug_inspect.frag");
}

void WorldFloor::onRender() {
    this->shader->use();
    ObjContainer* obj = this->objContainer;
    glm::mat4 modelM(1.f);
    // Move to side
    float objWidth = obj->GetObjSize().x;
    float objHeight = obj->GetObjSize().y;
    modelM = glm::scale(modelM, glm::vec3(objScale));
    modelM = glm::translate(modelM, -obj->GetOffsetCenter());
    // Align to top or bottom
    float align = this->align == ALIGN_BOTTOM ? 1 : -1;
    modelM = glm::translate(modelM, glm::vec3(0, align * objHeight/2, 0));
    shader->setMat4("model", modelM);
    // Draw object
    this->drawObject(obj, this->shader);
}

void WorldFloor::drawObject(ObjContainer* obj, Shader* shader) 
{
    // glm::vec3 rotatingPosition = GetRotatingPosition();
    int numShapes = obj->shapes.size();
    for (int i = 0; i < numShapes; ++i)
    {
        Shape currentShape = obj->shapes[i];
        // shader->setVec3("diffuse", currentShape.material.diffuse);
        // if(shader == simpleShader) {
        // } 
        // if(shader == lightingShader)
        // {
        //     shader->setVec3("scene.viewDir", glm::vec3(0,-1,0));
        //     shader->setVec3("scene.viewPos", this->Camera->GetCameraPosition());
        //     shader->setVec3("scene.rotatingPosition", rotatingPosition);

        //     shader->setInt("material.texture", 0);
        //     shader->setFloat("material.shininess", currentShape.material.shininess);
        //     shader->setVec3("material.diffuse", currentShape.material.diffuse);
        //     shader->setVec3("material.specular", currentShape.material.specular);
        //     shader->setVec3("material.ambient", currentShape.material.ambient);
        //     shader->EnableTexture(currentShape.textureDiffuseId);
        // }

        currentShape.DrawShape();
    }
}

