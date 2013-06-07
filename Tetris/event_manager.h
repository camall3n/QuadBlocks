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
#include "timer.h"
#include "world.h"

class EventManager
{
public:
    EventManager();
    
    void setController(Controller* controller);
    void setWorld(World* world);
    
    void activate();
    
private:
    Controller* controller;
    World* world;
    
    void LSReallyMovedUp();
};

#endif /* defined(__Tetris__event_manager__) */
