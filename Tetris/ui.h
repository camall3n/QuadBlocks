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
//#include <glm/glm.hpp>

class UI
{
public:
    UI();
    ~UI();
    
    void draw();
    void update();
    
private:
    WebCore* webCore;
    WebView* webView;
    
    void InitializeGLObjects();
    void DestroyGLObjects();
    
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
