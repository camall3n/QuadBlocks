//
//  controller.cpp
//  SDLJoystick
//
//  Created by Cam Allen on 11/4/12.
//

#include <iostream>
#include <math.h>

#include "controller.h"

#pragma mark Constants

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

const int XBOX_NUM_AXES = 6;

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

const int XBOX_NUM_BUTTONS = 15;

#pragma mark
#pragma mark Controller

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
{

}

Controller::~Controller()
{

}

void Controller::init(int whichJoystick)
{
    this->whichJoystick = whichJoystick;
}

void Controller::update()
{
    updateButtons();
    updateAxes();
}

void Controller::updateButtons()
{
    unsigned char buttons[XBOX_NUM_BUTTONS];
    int nButtons = glfwGetJoystickButtons(whichJoystick, buttons, XBOX_NUM_BUTTONS);
    if (nButtons == XBOX_NUM_BUTTONS) {
        
        for (int i=0; i<XBOX_NUM_BUTTONS; ++i) {
            switch (i) {
                case XBOX_BUTTON_LS: LS.update(buttons[i]); break;
                case XBOX_BUTTON_RS: RS.update(buttons[i]); break;
                    
                case XBOX_BUTTON_A: A.update(buttons[i]); break;
                case XBOX_BUTTON_B: B.update(buttons[i]); break;
                case XBOX_BUTTON_X: X.update(buttons[i]); break;
                case XBOX_BUTTON_Y: Y.update(buttons[i]); break;
                    
                case XBOX_BUTTON_LB: LB.update(buttons[i]); break;
                case XBOX_BUTTON_RB: RB.update(buttons[i]); break;
                    
                case XBOX_BUTTON_DPAD_UP:    Up.update(buttons[i]); break;
                case XBOX_BUTTON_DPAD_DOWN:  Down.update(buttons[i]); break;
                case XBOX_BUTTON_DPAD_LEFT:  Left.update(buttons[i]); break;
                case XBOX_BUTTON_DPAD_RIGHT: Right.update(buttons[i]); break;
                    
                case XBOX_BUTTON_BACK:  Back.update(buttons[i]); break;
                case XBOX_BUTTON_START: Start.update(buttons[i]); break;
                case XBOX_BUTTON_GUIDE: Guide.update(buttons[i]); break;
                    
                default: break;
            }
        }
    }
    else {
        printf("Failed to get joystick buttons. Expecting %d buttons, got %d\n",
               XBOX_NUM_BUTTONS, nButtons);
    }
}

void Controller::updateAxes()
{
    float positions[XBOX_NUM_AXES];
    int nPositions = glfwGetJoystickPos(whichJoystick, positions, XBOX_NUM_AXES);
    if (nPositions == XBOX_NUM_AXES) {
        LS.update(positions[XBOX_AXIS_LS_HORIZONTAL], positions[XBOX_AXIS_LS_VERTICAL]);
        RS.update(positions[XBOX_AXIS_RS_HORIZONTAL], positions[XBOX_AXIS_RS_VERTICAL]);
        
        LT.update(positions[XBOX_AXIS_LTRIGGER]);
        RT.update(-1 * positions[XBOX_AXIS_RTRIGGER]);
    }
    else {
        printf("Failed to get joystick buttons. Expecting %d buttons, got %d\n",
               XBOX_NUM_BUTTONS, nPositions);
    }
}

/*----------------------------------------------------------------------------
 * Controller::Button
 *----------------------------------------------------------------------------*/
#pragma mark
#pragma mark Button
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

void Controller::Button::update(unsigned char pressed)
{
    if (pressed == GLFW_PRESS) {
        _pressed = true;
    }
    else if (pressed == GLFW_RELEASE) {
        _pressed = false;
    }
}

bool Controller::Button::isPressed()
{
    return _pressed;
}

/*----------------------------------------------------------------------------
 * Controller::Thumbstick
 *----------------------------------------------------------------------------*/
#pragma mark
#pragma mark Thumbstick
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

void Controller::Thumbstick::update(unsigned char pressed)
{
    if (pressed == GLFW_PRESS) {
        _pressed = true;
    }
    else if (pressed == GLFW_RELEASE) {
        _pressed = false;
    }
}

void Controller::Thumbstick::update(float x, float y)
{
    _x = x;
    _y = y;
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
#pragma mark
#pragma mark Trigger
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


void Controller::Trigger::update(float z)
{
    _z = z;
}

float Controller::Trigger::z()
{
    if (_z > TRIGGER_DEADZONE)
        return _z;
    return 0.0;
}

