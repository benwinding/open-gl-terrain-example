#include "scene_components/WorldFloor.h"

WorldFloor::WorldFloor(float scale, bool alignBottom) {
    this->objScale = scale;
    this->alignBottom = alignBottom;
}

void WorldFloor::onSetup() {
    const char* fname = "res/models/cube-simple/cube-simple.obj";
    // const char* fname = "res/models/Barrel/Barrel02.obj";
    this->objContainer = new ObjContainer((char*) fname);
}

void WorldFloor::onRender(Shader* shader) {
    ObjContainer* obj = this->objContainer;
    glm::mat4 modelM(1.f);
    // Move to side
    float objWidth = obj->GetObjSize().x;
    float objHeight = obj->GetObjSize().y;
    modelM = glm::scale(modelM, glm::vec3(objScale));
    modelM = glm::translate(modelM, -obj->GetOffsetCenter());
    float align = this->alignBottom ? 1 : -1;
    modelM = glm::translate(modelM, glm::vec3(0, align * objHeight/2, 0));
    // Draw object
    shader->setMat4("model", modelM);

    this->drawObject(obj, shader);
}

