#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "glm/gtc/matrix_transform.hpp"
#include "graphics/InputState.h"

// Repsonsible For Tracking the players movements
class Player {
private:
    float turnIncrement;
    float pan, tilt;

    glm::vec3 location;

    float GetPlayerHeight();
    void MoveStraight(int dir);

    void PanLeft();
    void PanRight();
    void TiltUp();
    void TiltDown();

    float getTurnIncrement();
    float getMoveIncrement();

    void resetAcceleration();

    float incrementMove;
    float incrementTurn;

public:
    Player();
    void Reset();
    void Update(InputState userInput);

    glm::vec3 GetLocation3();
    glm::vec3 GetDirection3();
    float GetPan();
    float GetTilt();
};

#endif