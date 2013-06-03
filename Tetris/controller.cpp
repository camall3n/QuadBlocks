//
//  controller.cpp
//
//  Created by Cam Allen on 11/4/12.
//

#include <iostream>
#include <math.h>

#include "controller.h"


#pragma mark Constants
const float THUMBSTICK_DEADZONE = 0.15;
const float TRIGGER_DEADZONE    = 0.01;

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
//        printf("Failed to get joystick buttons. Expecting %d buttons, got %d\n", XBOX_NUM_BUTTONS, nButtons);
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
//        printf("Failed to get joystick buttons. Expecting %d buttons, got %d\n", XBOX_NUM_BUTTONS, nPositions);
    }
}

/*----------------------------------------------------------------------------
 * Controller::Button
 *----------------------------------------------------------------------------*/
#pragma mark
#pragma mark Button
Controller::Button::Button(int type)
{
    _type = type;
    _pressed = false;
}

void Controller::Button::update(unsigned char pressed)
{
    if (_pressed == false && pressed == GLFW_PRESS) {
        _pressed = true;
        signal.pressed();
    }
    else if (_pressed == true && pressed == GLFW_RELEASE) {
        _pressed = false;
        signal.released();
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
Controller::Thumbstick::Thumbstick(int type) :
    _type(type),
    _x(0.0),
    _y(0.0),
    _pressed(false),
    _moving(N_MOTIONS, false)
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
}

void Controller::Thumbstick::update(unsigned char pressed)
{
    if (_pressed == false && pressed == GLFW_PRESS) {
        _pressed = true;
        signal.pressed();
    }
    else if (_pressed == true && pressed == GLFW_RELEASE) {
        _pressed = false;
        signal.released();
    }
}

void Controller::Thumbstick::update(float x, float y)
{
    float old_x = this->x();
    float old_y = this->y();

    _x = x;
    _y = y;
    
    float new_x = this->x();
    float new_y = this->y();

    if ( (new_x != old_x) ||
         (new_y != old_y) )
    {
        signal.moved(new_x, new_y);
    }
    
    if ( new_x >= old_x + THUMBSTICK_DEADZONE )
    {
        _moving[MOTION_LEFT] = false;
        
        if ( !_moving[MOTION_RIGHT] && (new_x > 0) ) {
            signal.movedRight();
            _moving[MOTION_RIGHT] = true;
        }
    }
    else if ( new_x <= old_x - THUMBSTICK_DEADZONE )
    {
        _moving[MOTION_RIGHT] = false;
        
        if ( !_moving[MOTION_LEFT] && (new_x < 0) ) {
            signal.movedLeft();
            _moving[MOTION_LEFT] = true;
        }
    }
    
    if ( new_y > THUMBSTICK_DEADZONE && old_y < THUMBSTICK_DEADZONE)
    {
        signal.movedUp();
    }
    else if ( new_y < -THUMBSTICK_DEADZONE && old_y > -THUMBSTICK_DEADZONE)
    {
        signal.movedDown();
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
#pragma mark
#pragma mark Trigger
Controller::Trigger::Trigger(int type) :
    _moving(N_MOTIONS, false)
{
    _type = type;
    if (type == XBOX_TRIGGER_LEFT) {
        _axis = XBOX_AXIS_LTRIGGER;
    }
    else if (type == XBOX_TRIGGER_RIGHT) {
        _axis = XBOX_AXIS_RTRIGGER;
    }
    _z = 0.0;
}


void Controller::Trigger::update(float z)
{
    float old_z = this->z();
    
    _z = (z+1)/2;
    
    float new_z = this->z();
    
    if (new_z != old_z)
    {
        signal.moved(new_z);
    }
    
    if (new_z >= old_z + TRIGGER_DEADZONE)
    {
        _moving[MOTION_OUT] = false;
        
        if ( !_moving[MOTION_IN] ) {
            signal.pressed();
            _moving[MOTION_IN] = true;
        }
    }
    
    if (new_z <= old_z - TRIGGER_DEADZONE)
    {
        _moving[MOTION_IN] = false;
        
        if ( !_moving[MOTION_OUT] ) {
            signal.released();
            _moving[MOTION_OUT] = true;
        }
    }
}

float Controller::Trigger::z()
{
    if (_z > TRIGGER_DEADZONE)
        return _z;
    return 0.0;
}

