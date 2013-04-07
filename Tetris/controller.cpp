//
//  controller.cpp
//  SDLJoystick
//
//  Created by Cam Allen on 11/4/12.
//

#include <iostream>
#include <math.h>

#include "controller.h"

const int AXIS_MAX_VALUE = 32768;

const int UNINITIALIZED = -1;

enum XBOX_AXIS_TYPE {
    XBOX_AXIS_LS_HORIZONTAL = 0,
    XBOX_AXIS_LS_VERTICAL   = 1,
    XBOX_AXIS_RS_HORIZONTAL = 2,
    XBOX_AXIS_RS_VERTICAL   = 3,
    XBOX_AXIS_LTRIGGER      = 4,
    XBOX_AXIS_RTRIGGER      = 5
};

enum XBOX_THUMBSTICK_TYPE {
    XBOX_THUMBSTICK_LEFT  = 0,
    XBOX_THUMBSTICK_RIGHT = 1
};

enum XBOX_TRIGGER_TYPE {
    XBOX_TRIGGER_LEFT  = 0,
    XBOX_TRIGGER_RIGHT = 1
};

enum XBOX_BUTTON_TYPE {
    XBOX_BUTTON_A = 11,
    XBOX_BUTTON_B = 12,
    XBOX_BUTTON_X = 13,
    XBOX_BUTTON_Y = 14,
    
    XBOX_BUTTON_LB = 8,
    XBOX_BUTTON_RB = 9,
    
    XBOX_BUTTON_LS = 6,
    XBOX_BUTTON_RS = 7,
    
    XBOX_BUTTON_DPAD_UP    =  0,
    XBOX_BUTTON_DPAD_DOWN  =  1,
    XBOX_BUTTON_DPAD_LEFT  =  2,
    XBOX_BUTTON_DPAD_RIGHT =  3,
    
    XBOX_BUTTON_BACK  =  5,
    XBOX_BUTTON_START =  4,
    XBOX_BUTTON_GUIDE = 10
};

Controller::Controller() :
    LS(XBOX_THUMBSTICK_LEFT),
    RS(XBOX_THUMBSTICK_RIGHT),
    
    LT(XBOX_TRIGGER_LEFT),
    RT(XBOX_TRIGGER_RIGHT),
    
    A(XBOX_BUTTON_A),
    B(XBOX_BUTTON_B),
    X(XBOX_BUTTON_X),
    Y(XBOX_BUTTON_Y),
    
    LB(XBOX_BUTTON_LB),
    RB(XBOX_BUTTON_RB),
    
    Up(XBOX_BUTTON_DPAD_UP),
    Down(XBOX_BUTTON_DPAD_DOWN),
    Left(XBOX_BUTTON_DPAD_LEFT),
    Right(XBOX_BUTTON_DPAD_RIGHT),
    
    Back(XBOX_BUTTON_BACK),
    Start(XBOX_BUTTON_START),
    Guide(XBOX_BUTTON_GUIDE)
{ }

Controller::~Controller()
{ }

void Controller::processEvent(const SDL_Event &event)
{
    if (event.type == SDL_JOYBUTTONDOWN ||
        event.type == SDL_JOYBUTTONUP )
    {
        switch (event.jbutton.button) {
            case XBOX_BUTTON_LS: LS.processEvent(event); break;
            case XBOX_BUTTON_RS: RS.processEvent(event); break;
                
            case XBOX_BUTTON_A: A.processEvent(event); break;
            case XBOX_BUTTON_B: B.processEvent(event); break;
            case XBOX_BUTTON_X: X.processEvent(event); break;
            case XBOX_BUTTON_Y: Y.processEvent(event); break;
                
            case XBOX_BUTTON_LB: LB.processEvent(event); break;
            case XBOX_BUTTON_RB: RB.processEvent(event); break;
                
            case XBOX_BUTTON_DPAD_UP:    Up.processEvent(event); break;
            case XBOX_BUTTON_DPAD_DOWN:  Down.processEvent(event); break;
            case XBOX_BUTTON_DPAD_LEFT:  Left.processEvent(event); break;
            case XBOX_BUTTON_DPAD_RIGHT: Right.processEvent(event); break;
                
            case XBOX_BUTTON_BACK:  Back.processEvent(event); break;
            case XBOX_BUTTON_START: Start.processEvent(event); break;
            case XBOX_BUTTON_GUIDE: Guide.processEvent(event); break;
                
            default: break;
        }
    }
    else if (event.type == SDL_JOYAXISMOTION) {
        switch (event.jaxis.axis) {
            case XBOX_AXIS_LS_VERTICAL:     // fall through
            case XBOX_AXIS_LS_HORIZONTAL: LS.processEvent(event); break;
            case XBOX_AXIS_RS_VERTICAL:     // fall through
            case XBOX_AXIS_RS_HORIZONTAL: RS.processEvent(event); break;
            case XBOX_AXIS_LTRIGGER:      LT.processEvent(event); break;
            case XBOX_AXIS_RTRIGGER:      RT.processEvent(event); break;

                
            default:   break;
        }
    }
}


/*----------------------------------------------------------------------------
 * Controller::Button
 *----------------------------------------------------------------------------*/
Controller::Button::Button(int type)
{
    _buttonType = type;
    _pressed = false;
}
Controller::Button::Button()
{
    _buttonType = UNINITIALIZED;
    _pressed = false;
}

void Controller::Button::processEvent(const SDL_Event &event){
    if (event.jbutton.button == _buttonType) {
        if (event.type == SDL_JOYBUTTONDOWN) {
            _pressed = true;
        }
        else if (event.type == SDL_JOYBUTTONUP) {
            _pressed = false;
        }
    }
}

bool Controller::Button::isPressed()
{
    return _pressed;
}

/*----------------------------------------------------------------------------
 * Controller::Thumbstick
 *----------------------------------------------------------------------------*/
Controller::Thumbstick::Thumbstick(int type)
{
    if (type == XBOX_THUMBSTICK_LEFT) {
        _axisX = XBOX_AXIS_LS_HORIZONTAL;
        _axisY = XBOX_AXIS_LS_VERTICAL;
        _buttonType = XBOX_BUTTON_LS;
    }
    else if (type == XBOX_THUMBSTICK_RIGHT) {
        _axisX = XBOX_AXIS_RS_HORIZONTAL;
        _axisY = XBOX_AXIS_RS_VERTICAL;
        _buttonType = XBOX_BUTTON_RS;
    }
    
    _x = 0.0;
    _y = 0.0;
    _pressed = false;
}
Controller::Thumbstick::Thumbstick()
{
    _axisX = UNINITIALIZED;
    _axisY = UNINITIALIZED;
    _buttonType = UNINITIALIZED;
    _x = 0.0;
    _y = 0.0;
    _pressed = false;
}

void Controller::Thumbstick::processEvent(const SDL_Event &event){
    if (event.type == SDL_JOYAXISMOTION) {
        if (event.jaxis.axis == _axisX) {
            _x = (float)event.jaxis.value / AXIS_MAX_VALUE;
        }
        else if (event.jaxis.axis == _axisY){
            _y = (float)event.jaxis.value / AXIS_MAX_VALUE;
        }
    }
    else if (event.jbutton.button == _buttonType) {
        if (event.type == SDL_JOYBUTTONDOWN) {
            _pressed = true;
        }
        else if (event.type == SDL_JOYBUTTONUP) {
            _pressed = false;
        }
    }
}

float Controller::Thumbstick::x(){
    if (sqrt(_x*_x+_y*_y) > THUMBSTICK_DEADZONE)
        return _x;
    return 0.0;
}
float Controller::Thumbstick::y(){
    if (sqrt(_x*_x+_y*_y) > THUMBSTICK_DEADZONE)
        return _y;
    return 0.0;
}
bool Controller::Thumbstick::isPressed(){
    return _pressed;
}

/*----------------------------------------------------------------------------
 * Controller::Trigger
 *----------------------------------------------------------------------------*/
Controller::Trigger::Trigger(int type)
{
    if (type == XBOX_TRIGGER_LEFT) {
        _axis = XBOX_AXIS_LTRIGGER;
    }
    else if (type == XBOX_TRIGGER_RIGHT) {
        _axis = XBOX_AXIS_RTRIGGER;
    }
    _z = 0.0;
}
Controller::Trigger::Trigger()
{
    _axis = UNINITIALIZED;
    _z = 0.0;
}


void Controller::Trigger::processEvent(const SDL_Event &event)
{
    if (event.type == SDL_JOYAXISMOTION) {
        if (event.jaxis.axis == _axis) {
            _z = (float)event.jaxis.value / AXIS_MAX_VALUE;
        }
    }
}

float Controller::Trigger::z()
{
    if (_z > TRIGGER_DEADZONE)
        return _z;
    return 0.0;
}

