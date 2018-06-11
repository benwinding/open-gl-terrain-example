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
#include "scene/ObjSingle.h"
#include "scene/ObjInstanced.h"
#include "scene/MirrorBox.h"
#include "scene/Fire.h"
#include "scene/Fountain.h"
#include "scene/Terrain.h"

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
// 3. Multiple shaders; 
// TODO: 3.1 Multiple Lighting, sun, torches
// 4. Sky box
// 5. Environment box on mirror cube
// TODO: 6. Depth cue (fog)
// TODO: 7. Bump mapping, light mapping, parralax mapping
// 8. Generated terrain
// 8.1 Particle systems, water, fire
// TODO: 9. Height mapping
// TODO: 10. Alpha blending 
// TODO: 11. Sounds
// TODO: 12. Collision detection

std::vector<InstanceParams*> App::MakeTreeInstances(int instanceCount, glm::vec3 location, glm::vec3 size, float minSize) {
    glm::vec3 limitsMin = location;
    glm::vec3 limitsMax = size + location;

    std::vector<InstanceParams*> instanceList;
    for (int i = 0; i < instanceCount; ++i)
    {
        float randX = Random::randomFloat(limitsMin.x, limitsMax.x);
        float randZ = Random::randomFloat(limitsMin.z, limitsMax.z);
        float randSize = Random::randomFloat(minSize, size.y);
        InstanceParams* instance = new InstanceParams();
        instance->location = GetGroundPos(randX, -randSize*0.05, randZ);
        instance->scale = glm::vec3(randSize);
        instance->rotation = glm::vec3(0);

        instanceList.push_back(instance);
    }
    return instanceList;
}

glm::vec3 App::GetGroundPos(float x, float yOff, float z) 
{
    float terrainHeight = this->terrain->CalculateTerrainHeight(x, z);
    return glm::vec3(x, terrainHeight + yOff, z);
}

void App::loadSceneComponents() 
{
    glEnable(GL_DEPTH_TEST);
    // Terrain
    glm::vec3 terrainSize = glm::vec3(100.f,3,100.f);
    glm::ivec3 terrainGrid = glm::ivec3(50,0,50);
    glm::vec3 terrainLocation = glm::vec3(0,1,0) - terrainSize*0.5f; 
    glm::vec3 treesLocation = terrainLocation;
    glm::vec3 treesSize1 = terrainSize;
    treesSize1.y = 8;
    glm::vec3 treesSize2 = terrainSize;
    treesSize2.y = 12;

    this->terrain = new Terrain(terrainLocation, terrainSize, terrainGrid);
    this->sceneComponents.push_back(this->terrain);
    // Player
    this->player = new Player(GetGroundPos(0, 1, -5), 90, 90);
    // Barrels
    std::string dir1 = "./res/models/";
    this->sceneComponents.push_back(new ObjSingle(1, GetGroundPos(-2,0,0), dir1 + "Barrel/Barrel02.obj"));
    this->sceneComponents.push_back(new Fire(1.5, 0.9, 500, GetGroundPos(-2,0.9,0)));
    this->sceneComponents.push_back(new ObjSingle(1, GetGroundPos(2,0,0), dir1 + "Barrel/Barrel02.obj"));
    this->sceneComponents.push_back(new Fountain(1.5, 0.9, 500, GetGroundPos(2,0.8,0)));
    // Trees
    this->sceneComponents.push_back(new ObjInstanced(MakeTreeInstances(40, treesLocation, treesSize1, 6), dir1 + "tree/PineTree03.obj"));
    this->sceneComponents.push_back(new ObjInstanced(MakeTreeInstances(60, treesLocation, treesSize2, 6), dir1 + "pine/PineTransp.obj"));

    if (!RENDER_ENVIRONMENT)
        return;
    this->sceneComponents.push_back(new MirrorBox(2, GetGroundPos(0,1,0), &this->Camera));
    // Skybox must be last
    this->sceneComponents.push_back(new Skybox());
}

void App::Render() 
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    this->player->Update(userInput);

    glm::vec3 playerLocation = this->player->GetLocation3();
    this->Camera->updateFromPlayer(
        playerLocation, 
        this->player->GetDirection3()
    );
    glm::mat4 view = this->Camera->getViewMtx();

    float terrainHeight = GetGroundPos(playerLocation.x, 1, playerLocation.z).y;
    this->player->UpdateYPos(terrainHeight);

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
    projection = glm::perspective(glm::radians(fov), aspect, 0.005f, 1000.0f);
}
