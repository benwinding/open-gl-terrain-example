#define GLFW_INCLUDE_NONE

#include <iostream>
#include <stdio.h>
#include <string>
#include <sys/types.h>

#if defined(__EMSCRIPTEN__)
#include <GLES3/gl3.h>
#include <emscripten/emscripten.h>
#include <emscripten/html5.h>
#else
#include <GL/glew.h>
#endif

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

void mainLoop(void* arg)
{
    (void)arg;
    if (glfwWindowShouldClose(window)) {
#if defined(__EMSCRIPTEN__)
        emscripten_cancel_main_loop();
#endif
        glfwDestroyWindow(window);
        glfwTerminate();
        return;
    }
#if defined(__EMSCRIPTEN__)
    static double lastResizeCheck = -1.0;
    double now = emscripten_get_now();
    if (lastResizeCheck < 0.0 || now - lastResizeCheck > 200.0) {
        lastResizeCheck = now;
        int canvasWidth = 0;
        int canvasHeight = 0;
        if (emscripten_get_canvas_element_size("#canvas", &canvasWidth, &canvasHeight) == EMSCRIPTEN_RESULT_SUCCESS) {
            if (canvasWidth > 0 && canvasHeight > 0 && (canvasWidth != winX || canvasHeight != winY)) {
                winX = canvasWidth;
                winY = canvasHeight;
                TheApp->SetWindowSize(canvasWidth, canvasHeight);
                glViewport(0, 0, canvasWidth, canvasHeight);
            }
        }
    }
#endif
    TheApp->Render();
    glfwSwapBuffers(window);
    glfwPollEvents();
}

void initWindow() {
    glfwSetErrorCallback(error_callback);
    
    if (!glfwInit()) {
        exit(1);
    }

#if defined(__EMSCRIPTEN__)
    // WebGL2 maps to OpenGL ES 3.0
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
#else
    // Specify that we want OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_FLOATING, GL_TRUE);
#endif

    // Create the window and OpenGL context
    window = glfwCreateWindow(winX, winY, "Epic Model Viewer - (CGA3 - Part 1)", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(1);
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

#ifndef __EMSCRIPTEN__
    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW\n";
        exit(1);
    }
#endif

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
        ~~~~~~~~~ OpenGL Demo ~~~~~~~~~
        ~~~~~~~ By Ben Winding ~~~~~~~~

    Usage: 
        ./demo
        ./demo --disable-sound

    Keyboard:
        ESC  = Exit Program
        Arrows = Move around
        U = Tilt view up
        D = Tilt view down
        1 = First Person View
        2 = Top View

    )V0G0N";
    std::cout << helpScreen << std::endl;
}

int main(int argc, char **argv)
{
    bool shouldDisableSound = false;
    for (int count = 0; count < argc; count++) {
        std::string currentArg = std::string(argv[count]);
        if (currentArg == std::string("--disable-sound")) {
            shouldDisableSound = true;
        }
    }
#ifdef __EMSCRIPTEN__
    shouldDisableSound = true;
#endif
    if (!shouldDisableSound) {
        sound.init();
    }
    initWindow();
#ifdef __EMSCRIPTEN__
    int canvasWidth = 0;
    int canvasHeight = 0;
    if (emscripten_get_canvas_element_size("#canvas", &canvasWidth, &canvasHeight) == EMSCRIPTEN_RESULT_SUCCESS) {
        if (canvasWidth > 0 && canvasHeight > 0) {
            winX = canvasWidth;
            winY = canvasHeight;
            glViewport(0, 0, canvasWidth, canvasHeight);
        }
    }
#endif
    initOpengl();
    printHelp();
    // Parse program arguments, add to a vector
    std::string windowTitle = "OpenGL Demo - By Ben Winding";
    glfwSetWindowTitle(window, windowTitle.c_str());
    TheApp = new App(winX, winY, argc);

#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop_arg(mainLoop, nullptr, 0, true);
#else
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
#endif
}   
