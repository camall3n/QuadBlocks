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

//Screen attributes
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_BPP = 32;
const int FRAMES_PER_SECOND = 60;
const char* const WINDOW_TITLE = "Tetris";

void display();
void checkError();
void cleanup();
void init();
void initGLFW();


int main(int argc, const char * argv[])
{
    init();
    
	while( glfwGetKey( GLFW_KEY_ESC ) != GLFW_PRESS &&
          glfwGetWindowParam( GLFW_OPENED ) )
    {
        display();
    }
    
    cleanup();
    
    return 0;
}

void init() {
    initGLFW();
    //    initSDL();
}

void initGLFW() {
    
    //Initialize GLFW
    if( !glfwInit() )
    {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        exit(-1);
    }
    
    glfwOpenWindowHint(GLFW_FSAA_SAMPLES, 4);
    //    glfwOpenWindowHint(GLFW_WINDOW_NO_RESIZE, GL_TRUE);
    glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 3);
    glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 2);
    //    glfwOpenWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    //    glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
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
    
    //    glfwEnable(GLFW_STICKY_KEYS);
    //
    //    glfwSetWindowCloseCallback(&onClose);
    //    glfwSetWindowSizeCallback(&onResize);
    //    glfwSetKeyCallback(&onKey);
}


void display() {
    // Clear the screen
    glClearColor(1.f, 0, 0.5f, 1);
    glClearDepth(1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glfwGetTime();
    
    glfwSwapBuffers();
    checkError();
}


void cleanup() {
    glfwTerminate();
}


void checkError()
{
    GLenum error;
    while ( (error = glGetError()) != GL_NO_ERROR ) {
        printf("%s\n", gluErrorString(error));
    }
}