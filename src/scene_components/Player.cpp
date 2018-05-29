#include <cmath>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "scene_components/Player.h"
#include "utils/Logger.h"

Player::Player()
{
    this->Reset();
}

void Player::Reset() {
    this->location = glm::vec3(0, 1.f, -5.f);
    this->pan = 90;
    this->tilt = 90;
}

void Player::Update(InputState userInput) 
{
    int dir = userInput.GetMove();
    switch (dir) 
    {
        case MOVEFORWARD:
            MoveStraight(MOVEFORWARD);
            break;
        case MOVEBACKWARD:
            MoveStraight(MOVEBACKWARD);
            break;
        case MOVELEFT:
            PanLeft();
            break;
        case MOVERIGHT:
            PanRight();
            break;
        case NOMOVE:
            resetAcceleration();
            break;
    }
}

void Player::MoveStraight(int moveDir) 
{
    float incr = this->getMoveIncrement();
    glm::vec3 playerDir = this->GetDirection3();
    glm::vec3 playerLoc = this->GetLocation3();
    if (moveDir == MOVEFORWARD)
        this->location += playerDir * incr;
    if (moveDir == MOVEBACKWARD)
        this->location -= playerDir * incr;
}

float Player::GetPlayerHeight()
{
    static double startTime = glfwGetTime();
    double nowTime = startTime - glfwGetTime();
    return 1+0.04f*sin(nowTime);
}

void Player::resetAcceleration() 
{
    this->incrementMove = 0;
    this->incrementTurn = 0;
}

float Player::getMoveIncrement() 
{
    static float incrementMax = 0.15f;
    static float incrementStep = 0.02f;
    if (incrementMove < incrementMax)
        incrementMove += incrementStep;
    else
        incrementMove = incrementMax;
    return incrementMove;
}

float Player::getTurnIncrement() 
{
    static float incrementMax = 2.f;
    static float incrementStep = 0.1f;
    if (incrementTurn < incrementMax)
        incrementTurn += incrementStep;
    else
        incrementTurn = incrementMax;
    return incrementTurn;
}

// Move commands
void Player::PanLeft()
{
    this->pan -= this->getTurnIncrement();
    if (this->pan <= 0)
        this->pan = 360;
}
void Player::PanRight()
{
    this->pan += this->getTurnIncrement();
    if (this->pan >= 360)
        this->pan = 0;
}
void Player::TiltUp()
{
    this->tilt -= this->getTurnIncrement();
    if (this->tilt <= 30)
        this->tilt = 30;
}
void Player::TiltDown()
{
    this->tilt += this->getTurnIncrement();
    if (this->tilt >= 150)
        this->tilt = 150;
}

// Getters
glm::vec3 Player::GetLocation3()
{
    return this->location;
}

glm::vec3 Player::GetDirection3()
{
    float pan = glm::radians(this->GetPan());
    float tilt = glm::radians(this->GetTilt());

    float camX = sin(tilt)*cos(pan);
    float camY = cos(tilt);
    float camZ = sin(tilt)*sin(pan);

    return glm::vec3(camX, camY, camZ);
}

float Player::GetPan()
{
    return this->pan;
}

float Player::GetTilt()
{
    return this->tilt;
}
