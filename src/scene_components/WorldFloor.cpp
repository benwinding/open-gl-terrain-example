
#include "scene_components/WorldFloor.h"

void WorldFloor::onSetup() {
    const char* fname = "models/Barrel/Barrel02.obj";
    this->objContainer = new ObjContainer((char*) fname);
    this->objScale = 65;
}

void WorldFloor::onRender(Shader* shader) {
    ObjContainer* obj = this->objContainer;
    glm::mat4 modelM(1.f);
    // Move to side
    float objWidth = obj->GetObjSize().x;
    float transXCenter = 0;
    modelM = glm::scale(modelM, glm::vec3(objScale));
    modelM = glm::translate(modelM, -obj->GetOffsetCenter());
    modelM = glm::translate(modelM, glm::vec3(transXCenter, 0, 0));
    // Draw object
    shader->setMat4("model", modelM);

    this->drawObject(obj, shader);
}

