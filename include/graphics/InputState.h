/**
 A set of variables that records mouse activity.
 You can update the variables yourself or use the functions.
*/

#ifndef _INPUTSTATE_H_
#define _INPUTSTATE_H_

#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"

enum keyMove {
    MOVEFORWARD,
    MOVEBACKWARD,
    MOVELEFT,
    MOVERIGHT,
    NOMOVE,
};

struct InputState
{    
    InputState(): lMousePressed(false),
                  rMousePressed(false),
                  prevX(0), prevY(0),
                  deltaX(0), deltaY(0) {
                    resetKeys();
                  }

    // Is the mouse button currently being held down?
    bool lMousePressed;
    bool rMousePressed;

    // Last known position of the cursor
    float prevX;
    float prevY;

    // Accumulated change in cursor position. 
    float deltaX;
    float deltaY;

    // Update cursor variables based on new position x,y
    void update(float x, float y)
    {
        float xDiff = x - prevX;
        float yDiff = y - prevY;
        deltaX += xDiff;
        deltaY += yDiff;
        prevX = x;
        prevY = y;
    };

    // Read off the accumulated motion and reset it
    void readDeltaAndReset(float *x, float *y)
    {
        *x = deltaX;
        *y = deltaY;
        deltaX = 0;
        deltaY = 0;
    };

    float GetRotationAngle() {
        float angle;
        if (stateLeft)
            angle = -1;
        else if (stateRight)
            angle = 1;
        else
            angle = 0;
        return angle;
    }

    glm::vec3 GetMoveDir() {
        glm::vec3 dir;
        if (stateUp)
            dir = glm::vec3(0,0,1);
        else if (stateDown)
            dir = glm::vec3(0,0,-1);
        else
            dir = glm::vec3(0,0,0);
        return dir;
    };

    int GetMove() {
        if (stateUp)
            return MOVEFORWARD;
        else if (stateDown)
            return MOVEBACKWARD;
        else if (stateLeft)
            return MOVELEFT;
        else if (stateRight)
            return MOVERIGHT;
        return NOMOVE;
    };

    bool isMovePress() {
        return stateUp || stateDown || stateLeft || stateRight;
    };

    void pressedUp() {
        stateUp = true;
    };
    void pressedDown() {
        stateDown = true;
    };
    void pressedLeft() {
        stateLeft = true;
    };
    void pressedRight() {
        stateRight = true;
    };

    bool resetKeys() {
        stateUp = false;
        stateDown = false;
        stateLeft = false;
        stateRight = false;
    };

private:
    bool stateUp;
    bool stateDown;
    bool stateLeft;
    bool stateRight;
};

#endif // _INPUTSTATE_H_
