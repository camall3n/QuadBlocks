//
//  controller.h
//
//  Created by Cam Allen on 11/4/12.
//

#ifndef SDLJoystick_controller_h
#define SDLJoystick_controller_h

#include <GL/glew.h>
#include <GL/glfw.h>
#include <boost/signals2.hpp>
#include <vector>
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
        Button(int type = UNINITIALIZED);
        void update(unsigned char pressed);
        
        bool isPressed();
        
        struct signalList {
            bs2::signal<void ()> pressed;
            bs2::signal<void ()> released;
        } signal;
        
    private:
        bool _pressed;
        int _type;
    };
    
    class Thumbstick
    {
    public:
        Thumbstick(int type = UNINITIALIZED);
        void update(unsigned char pressed);
        void update(float x, float y);
        
        float x();
        float y();
        bool isPressed();
        
        struct signalList {
            bs2::signal<void ()> pressed;
            bs2::signal<void ()> released;
            bs2::signal<void (float x, float y)> moved;
            bs2::signal<void ()> movedUp;
            bs2::signal<void ()> movedDown;
            bs2::signal<void ()> movedLeft;
            bs2::signal<void ()> movedRight;
            bs2::signal<void ()> holdingUp;
            bs2::signal<void ()> holdingDown;
            bs2::signal<void ()> holdingLeft;
            bs2::signal<void ()> holdingRight;
        } signal;
        
    private:
        float _x;
        float _y;
        bool  _pressed;
        int   _axisX;
        int   _axisY;
        int   _buttonType;
        int   _type;
        enum THUMBSTICK_DIR {
            THUMBSTICK_LEFT = 0,
            THUMBSTICK_RIGHT,
            THUMBSTICK_UP,
            THUMBSTICK_DOWN,
            //--------------
            N_THUMBSTICK_DIRS
        };
        std::vector<bool> _moving;
    };
    
    class Trigger
    {
    public:
        Trigger(int type = UNINITIALIZED);
        void update(float z);
        
        float z();
        
        struct signalList {
            bs2::signal<void ()> pressed;
            bs2::signal<void ()> released;
            bs2::signal<void (float z)> moved;
        } signal;
        
    private:
        float _z;
        int   _axis;
        int   _type;
        enum TRIGGER_DIR {
            TRIGGER_IN = 0,
            TRIGGER_OUT,
            //--------------
            N_TRIGGER_DIRS
        };
        std::vector<bool> _moving;
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
