//
//  main.m
//  Tetris
//
//  Created by Cam Allen on 4/6/13.
//  Copyright (c) 2013 Cam Allen. All rights reserved.
//

#include <GL/glew.h>
#include <GL/glfw.h>
#include <OpenGL/OpenGL.h>

#include <stdio.h>
#include <stdlib.h>

#include "block.h"
#include "controller.h"
#include "mesh.h"
#include "ResourcePath.h"
#include "utility.h"
#include "world.h"

//Constants
#pragma mark Constants
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 800;
const int SCREEN_BPP = 32;
const int FRAMES_PER_SECOND = 60;
const char* const WINDOW_TITLE = "Tetris";

//Globals
#pragma mark
#pragma mark Globals
bool should_quit = false;
Controller controller;
World* w = NULL;

// Declarations
#pragma mark
#pragma mark Declarations
void init();
void input();
void update();
void display();
void cleanup();

void initGLFW();

// GLFW Callbacks
void GLFWCALL onKey(int key, int action);
void GLFWCALL onResize(int x, int y);
int  GLFWCALL onClose();

// Main
#pragma mark
#pragma mark Main
int main(int argc, char* argv[]) {
    
    init();
    
	while( !should_quit && glfwGetWindowParam(GLFW_OPENED) )
    {
        input();
//        update();
        display();
    }
    
    cleanup();
    
    return 0;
}

#pragma mark
#pragma mark Functions

void init() {
    initGLFW();
    w = new World();
}

void input() {
    controller.update();
}

void display() {
    // Clear the screen
    if (controller.Start.isPressed())
        glClearColor(1.f, 0, 0.5f, 1);
    else
        glClearColor(0, 0, 0, 0);
    glClearDepth(1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
//    glfwGetTime();
    
    w->draw();
    
    glfwSwapBuffers();
    checkError();
}

void cleanup() {
    delete w;
    glfwTerminate();
}


void initGLFW() {
    
    //Initialize GLFW
    if( !glfwInit() )
    {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        exit(-1);
    }
    
    glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 3);
    glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 2);
    glfwOpenWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwOpenWindowHint(GLFW_FSAA_SAMPLES, 8);
    glfwOpenWindowHint(GLFW_WINDOW_NO_RESIZE, GL_TRUE);
    
    // Open a window and create its OpenGL context
	if( !glfwOpenWindow( SCREEN_WIDTH, SCREEN_HEIGHT, 0,0,0,0, SCREEN_BPP,0, GLFW_WINDOW ) )
	{
		fprintf( stderr, "Failed to open GLFW window as an OpenGL 3.2 context.\n" );
		glfwTerminate();
		exit(-1);
	}
    
    glewExperimental = GL_TRUE;
    
    checkError();
    // Initialize GLEW
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		exit(-1);
	}
    GLenum error;
    if ( (error = glGetError()) == GL_INVALID_ENUM) {
        //ignore it.
    }
    else {
        printf("%s\n", gluErrorString(error));
    }
    
    glfwSetWindowTitle( WINDOW_TITLE );
    
    glfwSetWindowCloseCallback(&onClose);
    glfwSetWindowSizeCallback(&onResize);
    glfwSetKeyCallback(&onKey);
    
    glfwDisable(GLFW_MOUSE_CURSOR);
    glfwSetMousePos(0, 0);
    
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CW);
    checkError("while setting up face culling");
    
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LEQUAL);
    glDepthRange(0.f, 1.f);
    glEnable(GL_DEPTH_CLAMP);
    checkError("while setting up depth mask");
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    checkError("while setting up alpha blending");
    
    controller.init(0);
}

// Callbacks
#pragma mark
#pragma mark Callbacks

void onKey(int key, int action) {
    
    if (key==GLFW_KEY_ESC && action == GLFW_PRESS) {
        onClose();
    }
    
}

int onClose() {
    
    should_quit = true;
    return GL_TRUE;
}

void onResize(int x, int y) {
    glViewport(0, 0, x, y);
}

