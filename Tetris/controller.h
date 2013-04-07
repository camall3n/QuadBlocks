//
//  controller.h
//  SDLJoystick
//
//  Created by Cam Allen on 11/4/12.
//

#ifndef SDLJoystick_controller_h
#define SDLJoystick_controller_h

#include <SDL/SDL.h>

//#include "thumbstick.h"

const float THUMBSTICK_DEADZONE = 0.15;
const float TRIGGER_DEADZONE    = 0.01;

class Controller
{
public:
    class Button
    {
    public:
        Button();
        Button(int type);
        void processEvent(const SDL_Event &event);
        
        bool isPressed();
        
    private:
        bool _pressed;
        int _buttonType;
    };
    
    class Thumbstick
    {
    public:
        Thumbstick();
        Thumbstick(int type);
        void processEvent(const SDL_Event &event);
        
        float x();
        float y();
        bool isPressed();
        
    private:
        float _x;
        float _y;
        bool  _pressed;
        int   _axisX;
        int   _axisY;
        int   _buttonType;
    };
    
    class Trigger
    {
    public:
        Trigger();
        Trigger(int type);
        void processEvent(const SDL_Event &event);
        
        float z();
        
    private:
        float _z;
        int   _axis;
    };
    
public:
    Controller();
    ~Controller();
    void processEvent(const SDL_Event &event);
    
    Thumbstick LS;
    Thumbstick RS;
    
    Trigger LT;
    Trigger RT;
    
    Button A;
    Button B;
    Button X;
    Button Y;
    
    Button LB;
    Button RB;

    Button Up;
    Button Down;
    Button Left;
    Button Right;
    
    Button Back;
    Button Start;
    Button  Guide;
};


#endif
