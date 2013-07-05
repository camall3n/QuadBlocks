//
//  ui.cpp
//  Tetris
//
//  Created by Cam Allen on 7/3/13.
//  Copyright (c) 2013 Cam Allen. All rights reserved.
//

#include "ui.h"

#include <iostream>
#include <GL/glew.h>
#include <GL/glfw.h>
#include <Awesomium/STLHelpers.h>
#include <Awesomium/BitmapSurface.h>
using namespace Awesomium;

#include "constants.h"
#include "utility.h"

const float vertexData[] = {
    -1,  1, 0,
     1,  1, 0,
     1, -1, 0,
    -1, -1, 0,
};

const GLshort indexData[] = {
    0, 1, 2,
    0, 2, 3,
};

const int g_TexUnit = 0;
GLuint g_textureSampler = 0;

UI::UI() :
    webCore(NULL),
    webView(NULL)
{
    WebString my_string(WSLit("Hello World!"));
    
    std::cout << my_string << std::endl;
    
    webCore = WebCore::Initialize(WebConfig());
    if (!webCore) {
        std::cout << "no web core" << std::endl;
    }
    
    webView = webCore->CreateWebView(SCREEN_WIDTH, SCREEN_HEIGHT);
    if (!webView) {
        std::cout << "no web view" << std::endl;
    }
    
    WebURL url(WSLit("http://www.google.com"));    
    if (!url.IsValid())
        std::cerr << "Error! URL was unable to be parsed.";
    webView->LoadURL(url);
    
    while (webView->IsLoading())
        webCore->Update();
    
    InitializeGLObjects();
}

UI::~UI()
{
    DestroyGLObjects();
    WebCore::Shutdown();
    webCore = NULL;
}

void UI::draw()
{
    glUseProgram(_glProgram);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _glObject.textureBuffer);
    glBindSampler(0, _glObject.sampler);
//    glBindSampler(g_TexUnit, g_textureSampler);
    
    glBindVertexArray(_glObject.vertexArray);
    {
        glDrawElements(GL_TRIANGLES, 1*3, GL_UNSIGNED_SHORT, 0);
    }
    glBindVertexArray(0);
    
//    glBindSampler(g_TexUnit, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glUseProgram(0);
    
    checkError("during draw");
}

void UI::update()
{
    webCore->Update();
}

void UI::InitializeGLObjects()
{
    // Create and compile program
    _glProgram = LoadProgram( "ui.vert", "ui.frag" );
    checkError("during program compilation");
    
    _glAttrib.position = glGetAttribLocation(_glProgram, "position");
    
    _glUniform.texture = glGetUniformLocation(_glProgram, "uiTexture");
    checkError("during location finding");
    
    // Generate and fill GL buffers
    glGenBuffers(1, &_glObject.vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, _glObject.vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    glGenBuffers(1, &_glObject.indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _glObject.indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexData), indexData, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    checkError("during buffer creation");

    // Generate GL textures
    glGenTextures(1, &_glObject.textureBuffer);
    glBindTexture(GL_TEXTURE_2D, _glObject.textureBuffer);

    glUseProgram(_glProgram);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _glObject.textureBuffer);
    glUniform1i(_glUniform.texture, 0);
    glUseProgram(0);

//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    
    glBindTexture(GL_TEXTURE_2D, 0);
    checkError("during texture creation");

    // Generate GL samplers
    glGenSamplers(1, &_glObject.sampler);
    glSamplerParameteri(_glObject.sampler, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glSamplerParameteri(_glObject.sampler, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glSamplerParameteri(_glObject.sampler, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glSamplerParameteri(_glObject.sampler, GL_TEXTURE_WRAP_T, GL_REPEAT);
    checkError("during sampler creation");
    
    // Generate and configure VAO for the GL buffers
    glGenVertexArrays(1, &_glObject.vertexArray);
	glBindVertexArray(_glObject.vertexArray);

    glBindBuffer(GL_ARRAY_BUFFER, _glObject.vertexBuffer);
    glEnableVertexAttribArray(_glAttrib.position);
    glVertexAttribPointer(_glAttrib.position, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _glObject.indexBuffer);
    
    glBindTexture(GL_TEXTURE_2D, _glObject.textureBuffer);

    glBindVertexArray(0);
    checkError("during VAO creation");

    BitmapSurface* surface = (BitmapSurface*) webView->surface();
    if (surface) {
        glBindTexture(GL_TEXTURE_2D, _glObject.textureBuffer);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, surface->width(), surface->height(), GL_BGRA, GL_UNSIGNED_BYTE, surface->buffer());
        glBindTexture(GL_TEXTURE_2D, 0);
    } else {
        std::cout << "NO SURFACE!!" << std::endl;
    }
}

void UI::DestroyGLObjects()
{
    glDeleteBuffers(1, &_glObject.vertexBuffer);
    glDeleteBuffers(1, &_glObject.indexBuffer);
    glDeleteTextures(1, &_glObject.textureBuffer);
    glDeleteSamplers(1, &_glObject.sampler);
    glDeleteProgram(_glProgram);
    
    checkError("during GL object destruction");

}

GLuint UI::_glProgram;
UI::GLAttributes UI::_glAttrib;
UI::GLUniforms UI::_glUniform;
UI::GLObjects UI::_glObject;