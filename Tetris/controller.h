//
//  controller.h
//
//  Created by Cam Allen on 11/4/12.
//

#ifndef SDLJoystick_controller_h
#define SDLJoystick_controller_h

#include <GL/glfw.h>
#include <boost/signals2.hpp>
namespace bs2 = boost::signals2;

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

class Controller
{
public:
    class Button
    {
    public:
        Button();
        Button(int type);
        void update(unsigned char pressed);
        
        bool isPressed();
        
        struct signalList {
            bs2::signal<void (int type)> pressed;
            bs2::signal<void (int type)> released;
        } signal;
        
    private:
        bool _pressed;
        int _type;
    };
    
    class Thumbstick
    {
    public:
        Thumbstick();
        Thumbstick(int type);
        void update(unsigned char pressed);
        void update(float x, float y);
        
        float x();
        float y();
        bool isPressed();
        
        struct signalList {
            bs2::signal<void (int type)> pressed;
            bs2::signal<void (int type)> released;
            bs2::signal<void (float x, float y, int type)> moved;
        } signal;
        
    private:
        float _x;
        float _y;
        bool  _pressed;
        int   _axisX;
        int   _axisY;
        int   _buttonType;
        int   _type;
    };
    
    class Trigger
    {
    public:
        Trigger();
        Trigger(int type);
        void update(float z);
        
        float z();
        
        struct signalList {
            bs2::signal<void (int type)> pressed;
            bs2::signal<void (int type)> released;
            bs2::signal<void (float z, int type)> moved;
        } signal;
        
    private:
        float _z;
        int   _axis;
        int   _type;
    };
    
public:
    Controller();
    ~Controller();
    
    void init(int whichJoystick=0);
    void update();
    void updateButtons();
    void updateAxes();
    
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
    Button Guide;
    
private:
    int whichJoystick;
};


#endif
