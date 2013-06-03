//
//  timer.cpp
//  SDLJoystick
//
//  Created by Cam Allen on 11/4/12.
//
#include "timer.h"

//#include <GL/glew.h>
#include <GL/glfw.h>
//#include <OpenGL/OpenGL.h>

Timer::Timer()
{
    startTime = 0;
    pausedTime = 0;
    paused = false;
    started = false;
}

void Timer::start()
{
    started = true;
    paused = false;
    startTime = glfwGetTime();
}

void Timer::stop()
{
    started = false;
    paused = false;
}

void Timer::pause()
{
    if ( (started==true) && (paused==false) ) {
        paused = true;
        pausedTime = glfwGetTime() - startTime;
    }
}

void Timer::unpause()
{
    if (paused==true) {
        paused = false;
        startTime = glfwGetTime() - pausedTime;
        pausedTime = 0;
    }
}

int Timer::getTime()
{
    if (started==true) {
        if (paused==true) {
            return pausedTime;
        }
        else{
            return glfwGetTime() - startTime;
        }
    }
    return 0;
}

bool Timer::isStarted()
{
    return started;
}

bool Timer::isPaused()
{
    return paused;
}