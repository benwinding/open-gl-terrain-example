#ifndef __APP_H_
#define __APP_H_

#include "glm/glm.hpp"

#include "Shader.h"
#include "Viewer.h"
#include "ObjContainer.h"
#include "InputState.h"

enum debugViewType
{
    WIRE_FRAME,
    NORMAL,
    DIFFUSE
};

enum lightingModeType
{
    OVERHEAD_RED_LIGHT,
    CAMERA_LIGHT,
    ROTATING_YELLOW_LIGHT,
    TEXTURE_ONLY
};

/*
 The App class is Repsonsible Composing the program and 
 Drawing the scene
 */
class App {
public:
    App(int winX, int winY, std::vector<char*> objFilePath);

    void Render();
    void Key_callback(int key, int action);
    void MouseBtn_callback(int button, int action);
    void MouseMove_callback(double x, double y);
    void SetWindowSize(int x, int y);

private:
    int winX, winY;

    float objScale;
    glm::vec3 totalExtents;

    std::vector<ObjContainer *> objList;

    InputState mouseInput;

    void drawObject(ObjContainer* obj) ;

    Shader *shader;
    Shader *simpleShader;
    Shader *lightingShader;
    void setShaders();

    void loadObjFiles(std::vector<char*> objFilePath);

    void cycleDebugView();
    void cycleLighting();
    void toggleLightTexture();

    // Modes
    int currentDebugView;
    int currentLightingMode;
    int isTextureOnlyMode;    
    void setDebugView(int debug);
    void setLightingMode(int debug);
    void setTextureMode(bool textureMode);

    glm::mat4 projection;
    void updateProjection();

    ObjectViewer *Camera;
};

#endif
