
/**
 * Draws a single cube in front of the camera.
 * Toggles Projection matrix, and depth buffer.
 */

#define GLFW_INCLUDE_NONE

#include <iostream>
#include <stdio.h>
#include <string>
#include <sys/types.h>

#include <GLFW/glfw3.h>

#include "App.h"
#include "utils/Logger.h"
#include "utils/Sound.h"

GLFWwindow* window;
int winX = 800;
int winY = 640;

App* TheApp;
Sound sound = Sound();

void key_callback(GLFWwindow* window,
                  int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        sound.kill();
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

    glfwWindowHint(GLFW_FLOATING, GL_TRUE);

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

void printHelp() 
{
    const char * helpScreen = R"V0G0N(
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        ~~~~ Assignment 3 - Part 2 ~~~~
        ~~~~~~~ By Ben Winding ~~~~~~~~

    Usage: 
        ./assign3_part2

    Keyboard:
        ESC  = Exit Program

    )V0G0N";
    std::cout << helpScreen << std::endl;
}

int main(int argc, char **argv)
{
    bool shouldDisableSound = false;
    for (int count = 0; count < argc; count++) {
        const auto currentArg = std::string(argv[count]);
        const auto hasDisableSoundArg = currentArg.compare("--disable-sound") != 0;
        if (hasDisableSoundArg) {
            shouldDisableSound = true;
        }
    }
    if (!shouldDisableSound) {
        sound.init();
    }
    initWindow();
    initOpengl();
    printHelp();
    // Parse program arguments, add to a vector
    std::string windowTitle = "Assign3 - Part2";
    glfwSetWindowTitle(window, windowTitle.c_str());
    TheApp = new App(winX, winY, argc);

    while (!glfwWindowShouldClose(window))
    {
        glfwWindowHint(GLFW_FLOATING, GLFW_TRUE);
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
