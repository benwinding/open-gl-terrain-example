#include <cmath>
#include <cstdlib>
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

float wave1(float x) {
    return sin(x) + sin(x*2 + M_PI) + sin(x*3);
}

float wave2(float x) {
    return sin(x) + sin(x + 1.5*M_PI*x) + sin(x*3);
}

