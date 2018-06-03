#include <string>
#include <iostream>
#include <algorithm>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cmath>

#include "glm/gtc/matrix_transform.hpp"

#include "App.h"
#include "scene/WorldFloor.h"
#include "scene/Skybox.h"
#include "utils/Logger.h"

// Public API

App::App(int winX, int winY)
{
    this->SetWindowSize(winX, winY);
    this->loadSceneComponents();
    glm::vec3 fpvPos(0, 1.0f, 3.5f);
    this->CamFirstPersion = new ObjectViewer(fpvPos);
    this->CamTopView = new TopObjectViewer(fpvPos);
    this->Camera = this->CamFirstPersion;
}

// 1. Flat landscape, load obj file (Tree)
// 2. Switchable camera First Person, World View
// TODO: 3. Multiple shaders; Lighting, sun, torches
// 4. Sky box
// TODO: 5. Environment box on mirror cube
// TODO: 6. Depth cue (fog)
// TODO: 7. Bump mapping, light mapping, parralax mapping
// TODO: 8. Generated terrain
// TODO: 9. Height mapping
// TODO: 10. Alpha blending 
// TODO: 11. Sounds
// TODO: 12. Collision detection

void App::loadSceneComponents() {
    glEnable(GL_DEPTH_TEST);
    this->worldFloor2 = new WorldFloor(20, ALIGN_TOP);
    this->worldFloor2->onSetup();

    this->worldFloor = new WorldFloor(1, ALIGN_BOTTOM);
    this->worldFloor->onSetup();

    this->skyBox = new Skybox();
    this->skyBox->onSetup();

    this->player = new Player();
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

    this->worldFloor->render(view, projection);
    this->worldFloor2->render(view, projection);
    this->skyBox->render(view, projection);

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
    if (action == GLFW_RELEASE) 
        userInput.resetKeys();
    if (action == GLFW_PRESS || action == GLFW_REPEAT)
    {
        switch(key) 
        {
            case GLFW_KEY_UP:
                userInput.pressedUp();
                break;
            case GLFW_KEY_DOWN:
                userInput.pressedDown();
                break;
            case GLFW_KEY_LEFT:
                userInput.pressedLeft();
                break;
            case GLFW_KEY_RIGHT:
                userInput.pressedRight();
                break;
        }
    }
    if (action == GLFW_PRESS)
    {
        switch(key) 
        {
            case GLFW_KEY_1:
                this->Camera = this->CamFirstPersion;
                break;
            case GLFW_KEY_2:
                this->Camera = this->CamTopView;
                break;
        }
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
    float fov = 85;
    projection = glm::perspective(glm::radians(fov), aspect, 0.005f, 1000.0f );
}
