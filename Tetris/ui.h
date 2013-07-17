//
//  ui.h
//  Tetris
//
//  Created by Cam Allen on 7/3/13.
//  Copyright (c) 2013 Cam Allen. All rights reserved.
//

#ifndef __Tetris__ui__
#define __Tetris__ui__

#include <Awesomium/WebCore.h>
using namespace Awesomium;
#include <GL/glew.h>
#include <GL/glfw.h>
#include <string>

#include "constants.h"
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
    
    void TogglePause();
    void SelectNextMenuItem();
    void SelectPrevMenuItem();
    void SelectMenuItem(int selection);
    
private:
    WebCore* webCore;
    WebView* webView;
    bool isDirty;
    Timer newPointsTimer;
    Timer messageTimer;
    bool paused;
    
    void UpdateFields();
    void UpdateGLTexture();
    void InitializeGLObjects();
    void DestroyGLObjects();
    
    void SetValue(std::string id, std::string value);
    void SetValue(std::string id, int value);
    
    void SetVisible(std::string id, bool visible);
    void SelectNextElement(std::string id);
    void SelectPrevElement(std::string id);
    void SelectElement(std::string id, int selection);
    
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
    
    static GLuint _glProgram;
    static GLAttributes _glAttrib;
    static GLUniforms _glUniform;
    static GLObjects _glObject;
};

#endif /* defined(__Tetris__ui__) */
