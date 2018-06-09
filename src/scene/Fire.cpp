#include <cmath>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "scene/Fire.h"
#include "utils/Logger.h"

Fire::Fire(float fireHeight, float fireWidth, int particleCount, glm::vec3 location)
{
    this->particleCount = particleCount;
    this->fireHeight = fireHeight;
    this->fireWidth = fireWidth;    
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
    this->particles = new FireParticle[particleCount];
}

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

int minLife = 300;
int maxLife = 1000;
float minSpeed = 0.02;
float maxSpeed = 0.1;
int maxDist = maxLife * maxSpeed;
void respawnParticle(FireParticle* p) {
    srand(getTime());
    p->life = randomFloat(minLife, maxLife);
    p->age = randomFloat(0, p->life);
    p->speed = randomFloat(minSpeed, maxSpeed);
    p->temperature = randomFloat(0, 1);
    p->initx = randomFloat(-1, 1);
    p->initz = randomFloat(-1, 1);
    p->variance = randomFloat(-1, 1);
}

void Fire::render(glm::mat4 viewMtx, glm::mat4 projectionMtx) {
    // Set common uniforms
    this->shader->use();
    shader->setMat4("view", viewMtx);
    shader->setMat4("projection", projectionMtx);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glm::mat4 modelM;
    // Move to side
    modelM = glm::mat4(1.f);
    modelM = glm::translate(modelM, this->location);
    shader->setMat4("model", modelM);
    // Draw fire
    float initScale = 0.1;
    for (int i = 0; i < this->particleCount; ++i)
    {
        FireParticle *p = &this->particles[i];
        p->age = p->age + 1;
        if (p->age == p->life)
            respawnParticle(p);

        float percentAge = (float)p->age / (float)p->life;
        float percentHeight = (p->age * p->speed) / maxDist;
        float particleScale = initScale * (0.7*(1-percentHeight) + 0.3*(1-percentAge));

        float yLoc = fireHeight * percentHeight;
        float maxRadius = 0.25 * fireWidth * sin(M_PI * percentAge);
        float xLoc = p->initx * maxRadius;
        float zLoc = p->initz * maxRadius;
        glm::mat4 varianceM(1.0f);
        varianceM = glm::translate(varianceM, glm::vec3(xLoc, yLoc, zLoc));
        varianceM = glm::scale(varianceM, glm::vec3(particleScale));
        float randAngle = M_PI * p->variance;
        varianceM = glm::rotate(varianceM, randAngle, glm::vec3(randAngle, randAngle, randAngle));
        shader->setMat4("variance", varianceM);

        // Calculate randomish colour
        float percentFromCenter = glm::length(glm::vec2(xLoc, zLoc)) / maxRadius;
        glm::vec3 red = glm::vec3(1, 0.02, 0.02);
        glm::vec3 yellow = glm::vec3(1, 1, 0.02);
        glm::vec3 black = glm::vec3(0.1);
        glm::vec3 colour;
        colour = glm::mix(yellow, red,   0.1*percentHeight + 0.1*p->temperature + 0.8*percentFromCenter);
        colour = glm::mix(colour, black, 0.8*percentHeight + 0.2*percentAge);
        shader->setVec3("colour", colour);
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

