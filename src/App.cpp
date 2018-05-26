#include <string>
#include <iostream>
#include <algorithm>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cmath>

#include "glm/gtc/matrix_transform.hpp"

#include "App.h"
#include "Logger.h"
#include "Shape.h"
#include "SceneCalculations.h"

// Public API

App::App(int winX, int winY)
{
    this->setShaders();
    this->SetWindowSize(winX, winY);
    this->loadSceneComponents();
    // this->loadObjFiles(objFilePaths);
    glm::vec3 cameraPos(0.0f, 0.0f, 3.5f);
    this->Camera = new ObjectViewer(cameraPos);
    this->cycleLighting();
}

void App::loadSceneComponents() {
    const char* fname = "models/Barrel/Barrel02.obj";
    ObjContainer* newObj = new ObjContainer((char*) fname);
    this->objList.push_back(newObj);
    this->objScale = CalculateTotalScale(this->objList);
    this->totalExtents = CalculateBoundingBoxTotal(this->objList);
}

void App::Render() 
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    Camera->update(mouseInput);    
    // Set common uniforms
    shader->setMat4("projection", projection);
    shader->setMat4("view", this->Camera->getViewMtx());

    int objCount = this->objList.size();
    float totalWidth = totalExtents.x;
    float remainingWidth = 0;

    for (int i = 0; i < objCount; ++i)
    {
        ObjContainer* obj = this->objList[i];
        glm::mat4 modelM(1.f);
        // Move to side
        float objWidth = obj->GetObjSize().x;
        float transAllToSide = -(objWidth * 0.5f) - remainingWidth;
        float transAllToCenter = transAllToSide + (totalWidth * 0.5f);
        float transAllScaled = transAllToCenter;
        modelM = glm::scale(modelM, glm::vec3(objScale));
        modelM = glm::translate(modelM, -obj->GetOffsetCenter());
        modelM = glm::translate(modelM, glm::vec3(transAllScaled, 0, 0));
        // Draw object
        shader->setMat4("model", modelM);
        this->drawObject(obj);
        remainingWidth += objWidth;
    }
    glFlush();
}

void App::MouseBtn_callback(int button, int action)
{
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
        mouseInput.rMousePressed = true;
    }
    else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE) {
        mouseInput.rMousePressed = false;
    }
    else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        mouseInput.lMousePressed = true;
    }
    else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
        mouseInput.lMousePressed = false;
    }                
}

void App::MouseMove_callback(double x, double y)
{
    mouseInput.update((float)x, (float)y);
}

void App::Key_callback(int key, int action)
{
    if (action != GLFW_PRESS) 
        return;
    switch(key) 
    {
        case GLFW_KEY_B:
            cycleDebugView();
            break;
        case GLFW_KEY_S:
            cycleLighting();
            break;
        case GLFW_KEY_D:
            toggleLightTexture();
            break;
    }
}

void App::SetWindowSize(int x, int y)
{
    this->winX = x;
    this->winY = y;
    this->updateProjection();
}

// Private methods

void App::loadObjFiles(std::vector<char*> objFilePaths) 
{
    int objCount = objFilePaths.size();
    std::cout << objCount << " filepaths recieved, reading files." << std::endl;

    for (int i = 0; i < objCount; ++i)
    {
        char* objFilePath = objFilePaths[i];
        ObjContainer* newObj = new ObjContainer(objFilePath);
        PrintObjFileInfo(newObj);
        this->objList.push_back(newObj);
    }
    this->objScale = CalculateTotalScale(this->objList);
    this->totalExtents = CalculateBoundingBoxTotal(this->objList);
    Print("     Total Extents: ", totalExtents);
    Print("All Objects Scaled: ", objScale);
}

void App::drawObject(ObjContainer* obj) 
{
    glm::vec3 rotatingPosition = GetRotatingPosition();

    int numShapes = obj->shapes.size();
    for (int i = 0; i < numShapes; ++i)
    {
        Shape currentShape = obj->shapes[i];
        if(shader == simpleShader) {
            shader->setVec3("diffuse", currentShape.material.diffuse);
        } 
        if(shader == lightingShader)
        {
            shader->setVec3("scene.viewDir", glm::vec3(0,-1,0));
            shader->setVec3("scene.viewPos", this->Camera->GetCameraPosition());
            shader->setVec3("scene.rotatingPosition", rotatingPosition);

            shader->setInt("material.texture", 0);
            shader->setFloat("material.shininess", currentShape.material.shininess);
            shader->setVec3("material.diffuse", currentShape.material.diffuse);
            shader->setVec3("material.specular", currentShape.material.specular);
            shader->setVec3("material.ambient", currentShape.material.ambient);
            shader->EnableTexture(currentShape.textureDiffuseId);
        }

        currentShape.DrawShape();
    }
}

void App::setDebugView(int debug) 
{
    this->isTextureOnlyMode = false;
    this->shader = this->simpleShader;
    this->shader->use();
    switch (debug) {
        case WIRE_FRAME:
            Print("- Debug Mode 0 : WIRE_FRAME");
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            break;
        case NORMAL:
            Print("- Debug Mode 1 : NORMAL");
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            break;        
        case DIFFUSE:
            Print("- Debug Mode 2 : DIFFUSE");
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            break;
    }
    shader->setInt("debugMode", debug);
}

void App::setLightingMode(int lightingMode)
{
    this->isTextureOnlyMode = false;
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    this->shader = this->lightingShader;
    this->shader->use();
    switch (lightingMode) {
        case OVERHEAD_RED_LIGHT:
            Print("- Lighting Mode 0 : OVERHEAD_RED_LIGHT");
            break;            
        case CAMERA_LIGHT:
            Print("- Lighting Mode 1 : CAMERA_LIGHT");
            break;        
        case ROTATING_YELLOW_LIGHT:
            Print("- Lighting Mode 2 : ROTATING_YELLOW_LIGHT");
            break;
    }
    shader->setInt("scene.lightingMode", lightingMode);
}

void App::setTextureMode(bool textureMode)
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    this->shader = this->lightingShader;
    this->shader->use();
    if (textureMode) {
        Print("- TextureMode Mode 1 : TEXTURES ON");
        shader->setInt("scene.lightingMode", TEXTURE_ONLY);
    }
    else {
        Print("- TextureMode Mode 0 : TEXTURES OFF");
        setLightingMode(this->currentLightingMode);        
    }
}

void App::cycleDebugView()
{
    this->currentDebugView = (this->currentDebugView + 1) % 3 ;
    setDebugView(this->currentDebugView);
}

void App::cycleLighting() 
{
    this->currentLightingMode = (this->currentLightingMode + 1) % 3 ;
    setLightingMode(this->currentLightingMode);
}

void App::toggleLightTexture() 
{    
    this->isTextureOnlyMode = !this->isTextureOnlyMode;
    setTextureMode(this->isTextureOnlyMode);
}

void App::setShaders() 
{
    std::string prefix = "res/";
    this->simpleShader = new Shader(prefix + "debug_inspect.vert", prefix + "debug_inspect.frag");
    this->lightingShader = new Shader(prefix + "light-shading.vert", prefix + "light-shading.frag");
    this->shader = this->simpleShader;
}

void App::updateProjection()
{
    float aspect = (float) this->winX / this->winY;    
    float fov = 85;
    projection = glm::perspective(glm::radians(fov), aspect, 0.005f, 1000.0f );
}
