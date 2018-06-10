#include <string>
#include <iostream>
#include <algorithm>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cmath>

#include "glm/gtc/matrix_transform.hpp"

#include "utils/Logger.h"
#include "utils/Random.h"
#include "App.h"

#include "scene/Skybox.h"
#include "scene/WorldFloor.h"
#include "scene/ObjSingle.h"
#include "scene/ObjInstanced.h"
#include "scene/MirrorBox.h"
#include "scene/Fire.h"
#include "scene/Fountain.h"

bool RENDER_ENVIRONMENT;

// Public API
App::App(int winX, int winY, int argc)
{
    RENDER_ENVIRONMENT = argc == 1;
    this->SetWindowSize(winX, winY);
    float topViewHeight = 5.f;
    this->CamTopView = new TopObjectViewer(topViewHeight);
    this->CamFirstPersion = new ObjectViewer();
    this->Camera = this->CamFirstPersion;
    this->loadSceneComponents();
}

// 1. Flat landscape, load obj file (Tree)
// 2. Switchable camera First Person, World View
// TODO: 3. Multiple shaders; Lighting, sun, torches
// 4. Sky box
// 5. Environment box on mirror cube
// TODO: 6. Depth cue (fog)
// TODO: 7. Bump mapping, light mapping, parralax mapping
// TODO: 8. Generated terrain
// TODO: 9. Height mapping
// TODO: 10. Alpha blending 
// TODO: 11. Sounds
// TODO: 12. Collision detection

std::vector<InstanceParams*> MakeTreeInstances() {
    std::vector<InstanceParams*> instanceList;
    for (int i = 0; i < 20; ++i)
    {
        float randX = randomFloat(-10, 10);
        float randZ = randomFloat(-10, 10);
        float randSize = randomFloat(1, 8);
        InstanceParams* instance = new InstanceParams();
        instance->location = glm::vec3(randX, 0, randZ);
        instance->scale = glm::vec3(randSize);

        instanceList.push_back(instance);
    }
    return instanceList;
}

void App::loadSceneComponents() {
    glEnable(GL_DEPTH_TEST);
    this->player = new Player(glm::vec3(0,1,-5), 90, 90);
    this->sceneComponents.push_back(new WorldFloor(20, ALIGN_TOP));
    // Barrels
    std::string dir1 = "./res/models/";
    this->sceneComponents.push_back(new ObjSingle(1, glm::vec3(0,0,0), dir1 + "Barrel/Barrel02.obj"));
    this->sceneComponents.push_back(new Fire(1.5, 0.9, 500, glm::vec3(0,0.9,0)));
    this->sceneComponents.push_back(new ObjSingle(1, glm::vec3(2,0,0), dir1 + "Barrel/Barrel02.obj"));
    this->sceneComponents.push_back(new Fountain(1.5, 0.9, 500, glm::vec3(2,0.8,0)));
    // Trees
    this->sceneComponents.push_back(new ObjInstanced(MakeTreeInstances(), dir1 + "tree/PineTree03.obj"));
    this->sceneComponents.push_back(new ObjInstanced(MakeTreeInstances(), dir1 + "pine/PineTransp.obj"));

    if (!RENDER_ENVIRONMENT)
        return;
    this->sceneComponents.push_back(new MirrorBox(3, glm::vec3(-2,1,5), &this->Camera));
    // Skybox must be last
    this->sceneComponents.push_back(new Skybox()); 
}

void App::Render() 
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    this->player->Update(userInput);

    this->Camera->updateFromPlayer(
        this->player->GetLocation3(), 
        this->player->GetDirection3()
    );
    glm::mat4 view = this->Camera->getViewMtx();

    int numComponents = this->sceneComponents.size();
    for (int i = 0; i < numComponents; ++i)
    {
        this->sceneComponents.at(i)->render(view, projection);
    }
    glFlush();
}

void App::MouseBtn_callback(int button, int action)
{
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
        userInput.rMousePressed = true;
    else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
        userInput.rMousePressed = false;
    else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
        userInput.lMousePressed = true;
    else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
        userInput.lMousePressed = false;               
}

void App::MouseMove_callback(double x, double y)
{
    userInput.update((float)x, (float)y);
}

void App::Key_callback(int key, int action)
{
    bool repeatPress = action == GLFW_PRESS || action == GLFW_REPEAT;
    bool singlePress = action == GLFW_PRESS;

    switch(key)
    {
        case GLFW_KEY_UP:
            userInput.setUp(repeatPress);
            break;
        case GLFW_KEY_DOWN:
            userInput.setDown(repeatPress);
            break;
        case GLFW_KEY_LEFT:
            userInput.setLeft(repeatPress);
            break;
        case GLFW_KEY_RIGHT:
            userInput.setRight(repeatPress);
            break;
        case GLFW_KEY_MINUS:
        case GLFW_KEY_KP_SUBTRACT:
            this->CamTopView->zoomOut();
            break;
        case GLFW_KEY_EQUAL:
        case GLFW_KEY_KP_ADD:
            this->CamTopView->zoomIn();
            break;
    }

    if (singlePress)
    {
        if (key == GLFW_KEY_1)
            this->Camera = this->CamFirstPersion;
        if (key == GLFW_KEY_2)
            this->Camera = this->CamTopView;
    }
}

void App::SetWindowSize(int x, int y)
{
    this->winX = x;
    this->winY = y;
    this->updateProjection();
}

// Private methods

void App::updateProjection()
{
    float aspect = (float) this->winX / this->winY;    
    float fov = 75;
    projection = glm::perspective(glm::radians(fov), aspect, 0.005f, 1000.0f );
}
