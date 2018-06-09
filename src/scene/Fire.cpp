#include <cmath>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "scene/Fire.h"
#include "utils/Logger.h"

Fire::Fire(float scale, glm::vec3 location)
{
    this->scale = scale;
    this->location = location;
    this->onSetup();
}

void Fire::onSetup() {
    float cubeVertices[] = {
        // positions          // normals
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };
    // cube VAO
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

    this->shader = new Shader("res/fire.vert","res/fire.frag");
}

double getTime() {
    static double startTime = glfwGetTime();
    static double nowTime = startTime - glfwGetTime();
    return nowTime++;
}

double functJagged(float speed, double nowTime, double min, double max)
{
    static double height = max - min;
    static double a = 1 / speed;
    double mult = nowTime / a;
    return min + height * (1 + -mult + floor(mult));
}

double randSin(double nowTime, double min, double max)
{
    static double height = max - min;
    return min + height * (1 + cos(nowTime))/2;
}

float randomFloat(float a, float b) {
    float random = ((float) rand()) / (float) RAND_MAX;
    float diff = b - a;
    float r = random * diff;
    return a + r;
}

void Fire::render(glm::mat4 viewMtx, glm::mat4 projectionMtx) {
    // Set common uniforms
    this->shader->use();
    shader->setMat4("view", viewMtx);
    shader->setMat4("projection", projectionMtx);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glm::mat4 modelM;
    // Align to top or bottom
    float align = 1; // align bottom = 1, align top = -1
    float objHeight = 1;
    // Move to side
    modelM = glm::mat4(1.f);
    // modelM = glm::translate(modelM, glm::vec3(0, align * objHeight/2, 0));
    modelM = glm::translate(modelM, this->location);
    shader->setMat4("model", modelM);
    // Draw fire
    int particles = 100;
    float particleScale = 0.1;
    double fireHeight = this->scale;
    double fireRadius = 0.5;
    double timeSeed = 0.6 * getTime();
    for (int i = 0; i < particles; ++i)
    {
        // Init random seed
        srand(timeSeed + i);
        // Calculate random transformation matrix
        float fraction = (float)i / (float)particles;
        float initY = fireHeight * fraction;
        double offset = initY + randomFloat(-fireHeight, fireHeight);
        double randSpeed = randomFloat(0.01, 0.9);
        double jaggedY = functJagged(randSpeed, randSpeed*offset, 0, fireHeight);
        float randY = jaggedY + randomFloat(0, 0.01);
        float radiusPoint = randomFloat(0.8, 1) * 0.5 * fireRadius * sin(M_PI * randY / fireHeight) * fireHeight;
        float randX = randomFloat(-1, 1) * radiusPoint;
        float randZ = randomFloat(-1, 1) * radiusPoint;
        glm::mat4 varianceM(1.0f);
        varianceM = glm::scale(varianceM, glm::vec3(particleScale));
        varianceM = glm::translate(varianceM, glm::vec3(randX, randY, randZ) / particleScale);
        float randAngle = randomFloat(0, M_PI);
        varianceM = glm::rotate(varianceM, randAngle, glm::vec3(randAngle, 1, 0));
        shader->setMat4("variance", varianceM);
        // Calculate randomish colour
        glm::vec3 red = glm::vec3(1, 0.1, 0.1);
        glm::vec3 yellow = glm::vec3(1, 1, 0.1);
        glm::vec3 black = glm::vec3(0.1);
        shader->setVec3("colour", red);
        this->drawObject();
    }
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Fire::drawObject() 
{
    // cubes
    glBindVertexArray(cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}

