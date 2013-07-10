//
//  keyboard.cpp
//  Tetris
//
//  Created by Cam Allen on 7/9/13.
//  Copyright (c) 2013 Cam Allen. All rights reserved.
//

#include "keyboard.h"
#include <boost/foreach.hpp>

//std::list<int> Keyboard::Keys = std::list<int>();
typedef std::map<int, Keyboard::Key*> keymap_t;
extern keymap_t Keys;

Keyboard::Keyboard() :
    Escape(GLFW_KEY_ESC),
    F1(GLFW_KEY_F1),
    F2(GLFW_KEY_F2),
    F3(GLFW_KEY_F3),
    F4(GLFW_KEY_F4),
    F5(GLFW_KEY_F5),
    F6(GLFW_KEY_F6),
    F7(GLFW_KEY_F7),
    F8(GLFW_KEY_F8),
    F9(GLFW_KEY_F9),
    F10(GLFW_KEY_F10),
    F11(GLFW_KEY_F11),
    F12(GLFW_KEY_F12),

    Backquote('`'),
    Num1('1'),
    Num2('2'),
    Num3('3'),
    Num4('4'),
    Num5('5'),
    Num6('6'),
    Num7('7'),
    Num8('8'),
    Num9('9'),
    Num0('0'),
    Minus('-'),
    Equals('='),
    Backspace(GLFW_KEY_BACKSPACE),

    Tab(GLFW_KEY_TAB),
    Q('Q'),
    W('W'),
    E('E'),
    R('R'),
    T('T'),
    Y('Y'),
    U('U'),
    I('I'),
    O('O'),
    P('P'),
    BracketOpen('['),
    BracketClose(']'),
    Backslash('\\'),

    CapsLock(GLFW_KEY_CAPS_LOCK),
    A('A'),
    S('S'),
    D('D'),
    F('F'),
    G('G'),
    H('H'),
    J('J'),
    K('K'),
    L('L'),
    SemiColon(';'),
    Quote('\''),
    Return(GLFW_KEY_ENTER),

    LShift(GLFW_KEY_LSHIFT),
    Z('Z'),
    X('X'),
    C('C'),
    V('V'),
    B('B'),
    N('N'),
    M('M'),
    Comma(','),
    Period('.'),
    Slash('/'),
    RShift(GLFW_KEY_RSHIFT),

    LControl(GLFW_KEY_LCTRL),
    LAlt(GLFW_KEY_LALT),
    LSuper(GLFW_KEY_LSUPER),
    Space(GLFW_KEY_SPACE),
    RSuper(GLFW_KEY_RSUPER),
    RAlt(GLFW_KEY_RALT),
    RControl(GLFW_KEY_RCTRL),

    ArrowLeft(GLFW_KEY_LEFT),
    ArrowRight(GLFW_KEY_RIGHT),
    ArrowUp(GLFW_KEY_UP),
    ArrowDown(GLFW_KEY_DOWN)
{
    
}

void Keyboard::update()
{
    glfwPollEvents();
    
    BOOST_FOREACH( keymap_t::value_type i, Keys) {
        Key* kptr = i.second;
        kptr->update(glfwGetKey(i.first));
    }
}

/*----------------------------------------------------------------------------
 * Keyboard::Key
 *----------------------------------------------------------------------------*/
#pragma mark
#pragma mark Button
Keyboard::Key::Key(int type) :
    _type(type),
    _pressed(false)
{
    Keys.insert( std::pair<int, Key*>(type, this) );
//    Keys[type] = this;
}

void Keyboard::Key::update(unsigned char pressed)
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

bool Keyboard::Key::isPressed()
{
    return _pressed;
}


