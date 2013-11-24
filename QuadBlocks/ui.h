//
//  ui.h
//  QuadBlocks
//
//  Created by Cam Allen on 7/3/13.
//  Copyright (c) 2013 Cam Allen. All rights reserved.
//

#ifndef __QuadBlocks__ui__
#define __QuadBlocks__ui__

#include <Awesomium/WebCore.h>
using namespace Awesomium;
#include <GL/glew.h>
#include <GL/glfw.h>
#include <string>

#include "constants.h"
#include "soundboard.h"
#include "timer.h"


const int TEXTURE_WIDTH = SCREEN_WIDTH;
const int TEXTURE_HEIGHT = SCREEN_HEIGHT;

class UI
{
public:
    UI();
    ~UI();
    
    void draw();
    void update();
    
    void SetNewPoints(int points);
    void SetScore(int score);
    void SetLines(int lines);
    void SetLevel(int level);
    void SetTime(int minutes, int seconds);
    void DisplayAllClear();
    void DisplayLineClear(int lines);
    void DisplayTSpin(int lines, bool kick);
    
    void NewGame();
    void MainMenu();
    void TogglePause();
    void ToggleDevMode();
    
    void SetActiveMenu(std::string menu);
    void SelectNextMenuItem();
    void SelectPrevMenuItem();
    void SelectMenuItem(int selection);
    int  GetSelection();
    void ClickMenuItem();
    void UnClickMenuItem();
    
private:
    WebCore* webCore;
    WebView* webView;
    bool isDirty;
    Timer newPointsTimer;
    Timer messageTimer;
    Timer clickTimer;
    bool paused;
    bool devMode;
    Soundboard soundboard;
    
    void UpdateFields();
    void UpdateMenu();
    void UpdateGLTexture();
    void InitializeGLObjects();
    void DestroyGLObjects();
    
    void SetValue(std::string id, std::string value);
    void SetDevModeValue(std::string value);
    void SetValue(std::string id, int value);
    
    void SetVisible(std::string id, bool visible);
    void SelectNextElement(std::string id);
    void SelectPrevElement(std::string id);
    void SelectElement(std::string id, int selection);
    void SelectNone(std::string id);
    void ClickSelectedElement(std::string id);
    void UnClickSelectedElement(std::string id);
    
    struct GLAttributes {
        GLuint position;
    };
    struct GLUniforms {
        GLuint texture;
    };
    struct GLObjects {
        GLuint vertexArray;
        GLuint vertexBuffer;
        GLuint indexBuffer;
        GLuint textureBuffer;
        GLuint sampler;
    };
    
    std::string _activeMenu;
    
    static GLuint _glProgram;
    static GLAttributes _glAttrib;
    static GLUniforms _glUniform;
    static GLObjects _glObject;
};

#endif /* defined(__QuadBlocks__ui__) */
