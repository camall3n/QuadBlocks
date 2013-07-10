//
//  event_manager.h
//  Tetris
//
//  Created by Cam Allen on 6/2/13.
//  Copyright (c) 2013 Cam Allen. All rights reserved.
//

#ifndef __Tetris__event_manager__
#define __Tetris__event_manager__

#include <vector>

#include "controller.h"
#include "keyboard.h"
#include "timer.h"
#include "ui.h"
#include "world.h"


class EventManager
{
public:
    EventManager();
    
    void setController(Controller* controller);
    void setWorld(World* world);
    void setKeyboard(Keyboard* keyboard);
    void setUI(UI* ui);
    
    void activate();
    
private:
    Controller* controller;
    Keyboard* keyboard;
    World* world;
    UI* ui;
    
    void LSReallyMovedUp();
};

#endif /* defined(__Tetris__event_manager__) */
