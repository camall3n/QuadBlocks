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
#include "menu.h"
#include "timer.h"
#include "ui.h"
#include "world.h"


class EventManager
{
public:
    EventManager();
    
    void setController(Controller* controller);
    void setKeyboard(Keyboard* keyboard);
    void setMenu(Menu* menu);
    void setUI(UI* ui);
    void setWorld(World* world);
    
    void activate();
    
private:
    Controller* controller;
    Keyboard* keyboard;
    Menu* menu;
    UI* ui;
    World* world;
    bool paused;
    
    void LSReallyMovedUp();
    
    void UniversalSignals();
    void GameMode(bool activate);
    void MenuMode(bool activate);
    void TogglePause();
    void Exit();
};

#endif /* defined(__Tetris__event_manager__) */
