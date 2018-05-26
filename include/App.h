#ifndef __APP_H_
#define __APP_H_

#include "glm/glm.hpp"

#include "graphics/Shader.h"
#include "graphics/Viewer.h"
#include "graphics/ObjContainer.h"
#include "graphics/InputState.h"
#include "scene_components/WorldFloor.h"

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
    App(int winX, int winY);

    void Render();
    void Key_callback(int key, int action);
    void MouseBtn_callback(int button, int action);
    void MouseMove_callback(double x, double y);
    void SetWindowSize(int x, int y);

private:
    int winX, winY;

    WorldFloor* worldFloor;

    InputState mouseInput;

    Shader *shader;
    Shader *simpleShader;
    Shader *lightingShader;
    void setShaders();

    void loadSceneComponents();
    
    // Modes
    int currentDebugView;
    int currentLightingMode;
    int isTextureOnlyMode;    
    void setDebugView(int debug);
    void setLightingMode(int debug);
    void setTextureMode(bool textureMode);
    void cycleDebugView();
    void cycleLighting();
    void toggleLightTexture();

    glm::mat4 projection;
    void updateProjection();

    ObjectViewer *Camera;
};

#endif
