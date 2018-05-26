#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "graphics/Shape.h"
#include "graphics/Shader.h"

#include "scene_components/SceneComponent.h"

void SceneComponent::drawObject(ObjContainer* obj, Shader* shader) 
{
    // glm::vec3 rotatingPosition = GetRotatingPosition();

    int numShapes = obj->shapes.size();
    for (int i = 0; i < numShapes; ++i)
    {
        Shape currentShape = obj->shapes[i];
        // if(shader == simpleShader) {
            shader->setVec3("diffuse", currentShape.material.diffuse);
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
