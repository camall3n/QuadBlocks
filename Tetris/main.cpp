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

#include "controller.h"

//Constants
#pragma mark Constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_BPP = 32;
const int FRAMES_PER_SECOND = 60;
const char* const WINDOW_TITLE = "Tetris";

//Globals
#pragma mark
#pragma mark Globals
bool should_quit = false;
Controller controller;

// Declarations
#pragma mark
#pragma mark Declarations
void init();
void input();
void update();
void display();
void cleanup();

void checkError();
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
    
    glfwGetTime();
    
    glfwSwapBuffers();
    checkError();
}

void cleanup() {
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

    glfwOpenWindowHint(GLFW_FSAA_SAMPLES, 4);
    glfwOpenWindowHint(GLFW_WINDOW_NO_RESIZE, GL_TRUE);
    
    // Open a window and create its OpenGL context
	if( !glfwOpenWindow( SCREEN_WIDTH, SCREEN_HEIGHT, 0,0,0,0, SCREEN_BPP,0, GLFW_WINDOW ) )
	{
		fprintf( stderr, "Failed to open GLFW window.\n" );
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
    if (glGetError() == GL_INVALID_ENUM) {
        //ignore it.
    }
    
    glfwSetWindowTitle( WINDOW_TITLE );
    
    glfwSetWindowCloseCallback(&onClose);
    glfwSetWindowSizeCallback(&onResize);
    glfwSetKeyCallback(&onKey);
    
    glfwDisable(GLFW_MOUSE_CURSOR);
    glfwSetMousePos(0, 0);
    
    controller.init(0);
}


void checkError() {
    GLenum error;
    while ( (error = glGetError()) != GL_NO_ERROR ) {
        printf("%s\n", gluErrorString(error));
    }
}


// Callbacks
#pragma mark
#pragma mark Callbacks

void onKey(int key, int action) {
    
    // Apparently escape key breaks XBOX_START_BUTTON ??
    
//    if (key==GLFW_KEY_ESC && action == GLFW_PRESS) {
//        onClose();
//    }
    
}

int onClose() {
    
    should_quit = true;
    return GL_TRUE;
}

void onResize(int x, int y) {
    glViewport(0, 0, x, y);
}

