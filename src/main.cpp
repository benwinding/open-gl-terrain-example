
/**
 * Draws a single cube in front of the camera.
 * Toggles Projection matrix, and depth buffer.
 */

#define GLFW_INCLUDE_NONE

#include <iostream>
#include <vector>
#include <stdlib.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp> // ...so now that's defined we can import GLM itself.
#include "glm/gtc/matrix_transform.hpp" // Needed for the perspective() method

#include "App.h"

GLFWwindow* window;
int winX = 640;
int winY = 640;

App* TheApp;
    
void key_callback(GLFWwindow* window,
                  int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    TheApp->Key_callback(key, action);
}

void mouse_pos_callback(GLFWwindow* window, double x, double y)
{
    TheApp->MouseMove_callback(x,y);
}    

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    TheApp->MouseBtn_callback(button, action);
}

void error_callback(int error, const char* description){  std::cerr << description; }

void windowResize_callback(GLFWwindow *window, int x, int y) 
{
    TheApp->SetWindowSize(x, y);
    glViewport( 0, 0, x, y );
}

void initWindow() {
    glfwSetErrorCallback(error_callback);
    
    if (!glfwInit()) {
        exit(1);
    }

    // Specify that we want OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create the window and OpenGL context
    window = glfwCreateWindow(winX, winY, "Epic Model Viewer - (CGA3 - Part 1)", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(1);
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    
    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW\n";
        exit(1);
    }

    // Callback functions
    glfwSetKeyCallback(window, key_callback);
    glfwSetFramebufferSizeCallback(window, windowResize_callback);
    glfwSetCursorPosCallback(window, mouse_pos_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
}

void initOpengl() {
    // Initialise OpenGL state
    glClearColor(0.3f, 0.3f, 0.5f, 0.8f);
    glEnable(GL_DEPTH_TEST);
    glFrontFace(GL_CCW);
}

void printHelp() {
    const char * helpScreen = R"V0G0N(
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        ~~~~ Assignment 3 - Part 1 ~~~~
        ~~~~~~~ By Ben Winding ~~~~~~~~

    Usage: 
        ./assign3_part1 file1.obj
        ./assign3_part1 file1.obj file2.obj

        At least one obj filepath is expected by the program.

    Keyboard:
        B    = Cycle between debug modes:
               Mode 1 : Wireframe mode.
               Mode 2 : Vertex normal mode.
               Mode 3 : Diffuse colour mode.

        S    = Cycle between lighting modes:
               Mode 1 : An overhead 'red' directional light. 
               Mode 2 : A 'green' camera light.
               Mode 3 : A 'yellow' light, which orbits the model.

        D    = Lighting on/off

        ESC  = Exit Program

    )V0G0N";
    std::cout << helpScreen << std::endl;
}

int main(int argc, char **argv)
{
    initWindow();
    initOpengl();
    printHelp();
    std::vector<char*> objFileNames;
    const char* fileName = "models/Barrel/Barrel02.obj";
    objFileNames.push_back((char*)fileName);
    // Parse program arguments, add to a vector
    std::string windowTitle = "Assign3 Part2"; 
    glfwSetWindowTitle(window, windowTitle.c_str());
    TheApp = new App(winX, winY, objFileNames);

    while (!glfwWindowShouldClose(window))
    {
        TheApp->Render();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Clean up
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(0);

    return 0;
}   
