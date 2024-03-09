#include <cmath>
#include <cstdlib>
#include <random>
#include "utils/Time.h"
#include "utils/Random.h"

double Random::getTime() {
    static double startTime = Time::getTimeNow();
    static double nowTime = startTime - Time::getTimeNow();
    return nowTime++;
}

float Random::randomFloat(float min, float max) {
    // Create a random device
    std::random_device rd;
    // Initialize a Mersenne Twister pseudo-random number generator
    std::mt19937 gen(rd());
    // Create a uniform distribution in the range [min, max)
    std::uniform_real_distribution<> dis(min, max);
    // Generate a random number
    float random = dis(gen);
    return random;
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

