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
    lapTime = startTime;
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
        pausedTime = glfwGetTime();
    }
}

void Timer::unpause()
{
    if (paused==true) {
        paused = false;
        pausedTime = 0;
    }
}

double Timer::getTime()
{
    if (started==true) {
        if (paused==true) {
            return pausedTime;
        }
        else {
            return glfwGetTime() - startTime;
        }
    }
    return 0;
}

double Timer::getLapTime()
{
    if (started==true) {
        if (paused == true) {
            double oldLapTime = lapTime;
            lapTime = pausedTime;
            return pausedTime - oldLapTime;
        }
        else {
            double oldLapTime = lapTime;
            lapTime = glfwGetTime();
            return lapTime - oldLapTime;
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