//
//  keyboard.h
//  QuadBlocks
//
//  Created by Cam Allen on 7/9/13.
//  Copyright (c) 2013 Cam Allen. All rights reserved.
//

#ifndef __QuadBlocks__keyboard__
#define __QuadBlocks__keyboard__

#include <GL/glew.h>
#include <GL/glfw.h>
#include <boost/signals2.hpp>
namespace bs2 = boost::signals2;
#include <map>
#include <vector>

class Keyboard
{
public:
    class Key
    {
    private:
        static const int UNINITIALIZED = -1;

    public:
        Key(int type=UNINITIALIZED);
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
    
public:
    Keyboard();
    
    void init(int whichKeyboard=0);
    void update();
    
    Key Escape;
    Key F1;
    Key F2;
    Key F3;
    Key F4;
    Key F5;
    Key F6;
    Key F7;
    Key F8;
    Key F9;
    Key F10;
    Key F11;
    Key F12;
    
    Key Backquote;
    Key Num1;
    Key Num2;
    Key Num3;
    Key Num4;
    Key Num5;
    Key Num6;
    Key Num7;
    Key Num8;
    Key Num9;
    Key Num0;
    Key Minus;
    Key Equals;
    Key Backspace;
    
    Key Tab;
    Key Q;
    Key W;
    Key E;
    Key R;
    Key T;
    Key Y;
    Key U;
    Key I;
    Key O;
    Key P;
    Key BracketOpen;
    Key BracketClose;
    Key Backslash;
    
    Key CapsLock;
    Key A;
    Key S;
    Key D;
    Key F;
    Key G;
    Key H;
    Key J;
    Key K;
    Key L;
    Key SemiColon;
    Key Quote;
    Key Return;
    
    Key LShift;
    Key Z;
    Key X;
    Key C;
    Key V;
    Key B;
    Key N;
    Key M;
    Key Comma;
    Key Period;
    Key Slash;
    Key RShift;
    
    Key LControl;
    Key LAlt;
    Key LSuper;
    Key Space;
    Key RSuper;
    Key RAlt;
    Key RControl;
    Key ArrowLeft;
    Key ArrowRight;
    Key ArrowUp;
    Key ArrowDown;

private:
    int whichKeyboard;
};

#endif /* defined(__QuadBlocks__keyboard__) */
