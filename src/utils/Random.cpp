#include <cmath>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

double getTime() {
    static double startTime = glfwGetTime();
    static double nowTime = startTime - glfwGetTime();
    return nowTime++;
}

float randomFloat(float a, float b) {
    float random = ((float) rand()) / (float) RAND_MAX;
    float diff = b - a;
    float r = random * diff;
    return a + r;
}

