//
//  event_manager.cpp
//  Tetris
//
//  Created by Cam Allen on 6/2/13.
//  Copyright (c) 2013 Cam Allen. All rights reserved.
//

#include "event_manager.h"

#include <iostream>
#include "constants.h"

extern const int FRAMES_PER_SECOND;

EventManager::EventManager() :
    controller(NULL),
    world(NULL)
{
    
}

void EventManager::setController(Controller *controller)
{
    this->controller = controller;
}

void EventManager::setWorld(World *world)
{
    this->world = world;
}

void EventManager::activate()
{
    if (controller && world) {
        // Move Right
        controller->LS.signal.movedRight.connect(
            boost::bind( &World::queueMoveRight, world)
        );
        controller->RB.signal.pressed.connect(
            boost::bind( &World::queueMoveRight, world)
        );
        controller->Right.signal.pressed.connect(
            boost::bind( &World::queueMoveRight, world)
        );
        
        // Move Left
        controller->LS.signal.movedLeft.connect(
            boost::bind( &World::queueMoveLeft, world)
        );
        controller->LB.signal.pressed.connect(
            boost::bind( &World::queueMoveLeft, world)
        );
        controller->Left.signal.pressed.connect(
            boost::bind( &World::queueMoveLeft, world)
        );
        
        // Rotate CW
        controller->RT.signal.pressed.connect(
            boost::bind( &World::queueRotateCW, world)
        );
        
        // Rotate CCW
        controller->LT.signal.pressed.connect(
            boost::bind( &World::queueRotateCCW, world)
        );
        
        // Hard Drop
        controller->A.signal.pressed.connect(
            boost::bind( &World::queueHardDrop, world)
        );

        // Soft Drop
        controller->LS.signal.movedDown.connect(
            boost::bind( &World::queueSoftDrop, world)
        );
        
        // Hold
        controller->Y.signal.pressed.connect(
            boost::bind( &World::queueHold, world)
        );
        
        // Pause
        controller->Start.signal.pressed.connect(
            boost::bind( &World::togglePause, world)
        );
    }
    else {
        if (!controller) {
            std::cerr << "Error: Event Manager can't activate without a controller."
                      << std::endl;
        }
        if (!world) {
            std::cerr << "Error: Event Manager can't activate without a world."
            << std::endl;
        }
    }
}


void EventManager::LSReallyMovedUp()
{
    if (controller->LS.y() > controller->LS.x()) {
        world->queueHardDrop();
    }
}
