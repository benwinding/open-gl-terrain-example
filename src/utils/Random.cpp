#include <cmath>
#include <cstdlib>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "utils/Random.h"

double Random::getTime() {
    static double startTime = glfwGetTime();
    static double nowTime = startTime - glfwGetTime();
    return nowTime++;
}

float Random::randomFloat(float a, float b) {
    float random = ((float) rand()) / (float) RAND_MAX;
    float diff = b - a;
    float r = random * diff;
    return a + r;
}

float Random::wave1(float x) {
    return sin(x) + sin(x*2 + M_PI) + sin(x*3);
}

float Random::wave2(float x) {
    return sin(x) + sin(x + 1.5*M_PI*x) + sin(x*3);
}

float Random::wave(float x, float y) {
    static int partCount = 3;
    float wave = 0;
    wave += sin(x)*sin(y) / partCount;
    wave += sin(x/3 + y)*sin(y/2) / partCount;
    wave += sin(x/5)*cos(y/11 + x) / partCount;
    return wave/2;
}

