//
//  ui.cpp
//  Tetris
//
//  Created by Cam Allen on 7/3/13.
//  Copyright (c) 2013 Cam Allen. All rights reserved.
//

#include "ui.h"

#include <boost/lexical_cast.hpp>
#include <iostream>
#include <GL/glew.h>
#include <GL/glfw.h>
#include <Awesomium/STLHelpers.h>
#include <Awesomium/BitmapSurface.h>
#include <Awesomium/WebCore.h>
using namespace Awesomium;
#include <unistd.h>

#include "utility.h"
#include "ResourcePath.h"

const float vertexData[] = {
    -1,  1, 0,
     1,  1, 0,
     1, -1, 0,
    -1, -1, 0,
};

const GLshort indexData[] = {
    0, 1, 3,
    1, 2, 3,
};

const int g_TexUnit = 0;
GLuint g_textureSampler = 0;

static WebURL FileURL(const char * filename) {
    WebString prefix(WSLit("file://"));
    WebString path(WSLit(ResourcePath(filename).c_str()));
    
    return WebURL(prefix.Append(path));
}
static WebURL FileURL(const std::string filename) {
    return FileURL(filename.c_str());
}

WebConfig *wc;

UI::UI() :
    webCore(NULL),
    webView(NULL),
    isDirty(true),
    paused(false),
    devMode(false)
{
    std::cout << "Leaking memory: Awesomium's ~WebConfig() descructor causes EXC_BAD_ACCESS (though only on perigee), so we 'new' and never 'delete'." << std::endl;
    wc = new WebConfig();
    webCore = WebCore::Initialize(*wc);
    if (!webCore) {
        std::cout << "no web core" << std::endl;
    }
    
    webView = webCore->CreateWebView(TEXTURE_WIDTH, TEXTURE_HEIGHT);
    if (!webView) {
        std::cout << "no web view" << std::endl;
    }
        
    WebURL url = FileURL("ui.html");
    if (!url.IsValid())
        std::cerr << "Error! URL was unable to be parsed.";
    webView->LoadURL(url);
    webView->SetTransparent(true);
    
    while (webView->IsLoading())
        webCore->Update();

//    usleep(20000);
//    sleep(2);
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
//    glBindSampler(0, _glObject.sampler);
    
    glBindVertexArray(_glObject.vertexArray);
    {
        glDrawElements(GL_TRIANGLES, 2*3, GL_UNSIGNED_SHORT, 0);
    }
    glBindVertexArray(0);
    
    glBindTexture(GL_TEXTURE_2D, 0);
    glUseProgram(0);
    
    checkError("during draw");
}

void UI::update()
{
    if (!paused) {
        UpdateFields();
    }
    else {
        UpdateMenu();
    }
    webCore->Update();
    
//    if (isDirty) {
//        isDirty = false;
    UpdateGLTexture();
//    }
}

static const double NEW_POINTS_DISPLAY_TIME = 0.5;
static const double MESSAGE_DISPLAY_TIME = 0.5;
void UI::UpdateFields()
{
    if (newPointsTimer.getTime() > NEW_POINTS_DISPLAY_TIME) {
        newPointsTimer.stop();
        SetValue("new_points", "");
    }
    if (messageTimer.getTime() > MESSAGE_DISPLAY_TIME) {
        messageTimer.stop();
        SetValue("message", "");
    }
}
static const double CLICK_TIME = 0.1;
void UI::UpdateMenu()
{
    if (clickTimer.getTime() > CLICK_TIME) {
        clickTimer.stop();
        UnClickMenuItem();
    }
}

void UI::UpdateGLTexture()
{
    BitmapSurface* surface = (BitmapSurface*) webView->surface();
    if (surface) {
        glBindTexture(GL_TEXTURE_2D, _glObject.textureBuffer);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, surface->width(), surface->height(), GL_BGRA, GL_UNSIGNED_BYTE, surface->buffer());
        glBindTexture(GL_TEXTURE_2D, 0);
    } else {
        std::cout << "NO SURFACE!!" << std::endl;
    }
}

void UI::SetValue(std::string id, std::string value)
{
    WebString none(WSLit(""));

    std::string command;
    command = "getValue('" + id + "');";
    
    JSValue prior = webView->ExecuteJavascriptWithResult(WSLit(command.c_str()), none);
    if (prior.ToString() == WSLit(value.c_str())) {
        return;
    }
    
    isDirty = true;
    command = "setValue('" + id + "', '" + value + "');";
    WebString script(WSLit(command.c_str()));
    JSValue j = webView->ExecuteJavascriptWithResult(script, none);
    if (!j.IsBoolean() || !j.ToBoolean()) {
        std::cerr << "JavaScript Error" << std::endl;
    }
}
void UI::SetValue(std::string id, int value) {
    SetValue(id, boost::lexical_cast<std::string>(value));
}

void UI::SetVisible(std::string id, bool visible)
{
    WebString none(WSLit(""));
    
    std::string command;
    command = "setVisible('" + id + "', "+ boost::lexical_cast<std::string>(visible) + ");";
    
    JSValue j = webView->ExecuteJavascriptWithResult(WSLit(command.c_str()), none);
    if (!j.IsBoolean() || !j.ToBoolean()) {
        std::cerr << "JavaScript Error" << std::endl;
    }
    isDirty = true;
}

void UI::SelectNextElement(std::string id)
{
    WebString none(WSLit(""));
    
    std::string command;
    command = "selectNextElement('" + id + "');";
    
    JSValue j = webView->ExecuteJavascriptWithResult(WSLit(command.c_str()), none);
    if (!j.IsBoolean() || !j.ToBoolean()) {
        std::cerr << "JavaScript Error" << std::endl;
    }
    isDirty = true;
}
void UI::SelectPrevElement(std::string id)
{
    WebString none(WSLit(""));
    
    std::string command;
    command = "selectPrevElement('" + id + "');";
    
    JSValue j = webView->ExecuteJavascriptWithResult(WSLit(command.c_str()), none);
    if (!j.IsBoolean() || !j.ToBoolean()) {
        std::cerr << "JavaScript Error" << std::endl;
    }
    isDirty = true;
}
void UI::SelectElement(std::string id, int selection)
{
    WebString none(WSLit(""));
    
    std::string command;
    command = "selectElement('" + id + "', '" + boost::lexical_cast<std::string>(selection) + "');";
    
    JSValue j = webView->ExecuteJavascriptWithResult(WSLit(command.c_str()), none);
    if (!j.IsBoolean() || !j.ToBoolean()) {
        std::cerr << "JavaScript Error" << std::endl;
    }
    isDirty = true;
}
int UI::GetSelection()
{
    WebString none(WSLit(""));
    
    std::string command;
    command = "getSelectedElement('menu');";
    
    JSValue j = webView->ExecuteJavascriptWithResult(WSLit(command.c_str()), none);
    if (!j.IsInteger() || j.ToInteger() < 0) {
        std::cerr << "JavaScript Error" << std::endl;
    }
    return j.ToInteger();
}

void UI::ClickSelectedElement(std::string id)
{
    WebString none(WSLit(""));
    
    std::string command;
    command = "clickSelectedElement('" + id + "');";
    
    JSValue j = webView->ExecuteJavascriptWithResult(WSLit(command.c_str()), none);
    if (!j.IsBoolean() || !j.ToBoolean()) {
        std::cerr << "JavaScript Error" << std::endl;
    }
    isDirty = true;
}
void UI::UnClickSelectedElement(std::string id)
{
    WebString none(WSLit(""));
    
    std::string command;
    command = "unClickSelectedElement('" + id + "');";
    
    JSValue j = webView->ExecuteJavascriptWithResult(WSLit(command.c_str()), none);
    if (!j.IsBoolean() || !j.ToBoolean()) {
        std::cerr << "JavaScript Error" << std::endl;
    }
    isDirty = true;
}


void UI::ClickMenuItem()
{
    ClickSelectedElement("menu");
    if (clickTimer.isStarted()) {
        clickTimer.stop();
    }
    clickTimer.start();
}

void UI::UnClickMenuItem()
{
    UnClickSelectedElement("menu");
}

void UI::SelectNextMenuItem() {
    SelectNextElement("menu");
}
void UI::SelectPrevMenuItem() {
    SelectPrevElement("menu");
}
void UI::SelectMenuItem(int selection) {
    SelectElement("menu", selection);
}

void UI::SetNewPoints(int points) {
    if (newPointsTimer.isStarted()) {
        newPointsTimer.stop();
    }
    newPointsTimer.start();
    SetValue("new_points", "+" + boost::lexical_cast<std::string>(points));
}
void UI::SetScore(int score) {
    SetValue("score", score);
}
void UI::SetLines(int lines) {
    SetValue("lines", lines);
}
void UI::SetLevel(int level) {
    SetValue("level", level);
}
void UI::SetTime(int minutes, int seconds) {
    assert(minutes >= 0);
    assert(seconds >= 0 && seconds < 60);

    std::string time = boost::lexical_cast<std::string>(minutes) + ":";
    if (seconds < 10) {
        time += "0";
    }
    time += boost::lexical_cast<std::string>(seconds);

    SetValue("time", time);
}
void UI::DisplayAllClear() {
    if (messageTimer.isStarted()) {
        messageTimer.stop();
    }
    messageTimer.start();
    SetValue("message", "All Clear");
}
void UI::DisplayLineClear(int lines) {
    if (messageTimer.isStarted()) {
        messageTimer.stop();
    }
    messageTimer.start();
    
    switch (lines) {
        case 4:
            SetValue("message", "Tetris");
            break;
        case 3:
            SetValue("message", "Triple");
            break;
        case 2:
            SetValue("message", "Double");
            break;
        case 1:
            SetValue("message", "Single");
            break;
            
        default:
            SetValue("message", "");
            break;
    }
}
void UI::DisplayTSpin(int lines, bool kick) {
    if (messageTimer.isStarted()) {
        messageTimer.stop();
    }
    messageTimer.start();
    
    switch (lines) {
        case 3:
            SetValue("message", "T-Spin Triple");
            break;
        case 2:
            SetValue("message", "T-Spin Double");
            break;
        case 1:
            if (kick) {
                SetValue("message", "Kick T-Spin Single");
            }
            else {
                SetValue("message", "T-Spin Single");
            }
            break;
        case 0:
            if (kick) {
                SetValue("message", "Kick T-Spin");
            }
            else {
                SetValue("message", "T-Spin");
            }
            break;
            
        default:
            SetValue("message", ""); break;
    }
}

void UI::TogglePause()
{
    if (paused) {
        paused = false;
        SetVisible("main", false);
    }
    else {
        paused = true;
        SelectMenuItem(0);
        SetVisible("main", true);
    }
    isDirty = true;
}

void UI::ToggleDevMode()
{
    devMode = !devMode;
    if (devMode) {
        SetValue("dev_mode", "Dev Mode (on)");
    }
    else {
        SetValue("dev_mode", "Dev Mode (off)");
    }
    isDirty = true;
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

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, TEXTURE_WIDTH, TEXTURE_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    
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
