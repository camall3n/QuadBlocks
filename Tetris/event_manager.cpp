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
            boost::bind( &World::moveRight, world)
        );
        controller->RB.signal.pressed.connect(
            boost::bind( &World::moveRight, world)
        );
        controller->Right.signal.pressed.connect(
            boost::bind( &World::moveRight, world)
        );
        
        // Move Left
        controller->LS.signal.movedLeft.connect(
            boost::bind( &World::moveLeft, world)
        );
        controller->LB.signal.pressed.connect(
            boost::bind( &World::moveLeft, world)
        );
        controller->Left.signal.pressed.connect(
            boost::bind( &World::moveLeft, world)
        );
        
        // Rotate CW
        controller->RT.signal.pressed.connect(
            boost::bind( &World::rotateCW, world)
        );
        
        // Rotate CCW
        controller->LT.signal.pressed.connect(
            boost::bind( &World::rotateCCW, world)
        );
        
        // Hard Drop
        controller->LS.signal.movedUp.connect(
//            boost::bind( &World::hardDrop, world)
            boost::bind( &World::moveUp, world)
        );

        // Soft Drop
        controller->LS.signal.movedDown.connect(
//            boost::bind( &World::softDrop, world)
            boost::bind( &World::moveDown, world)
        );
        
        // Hold
        controller->A.signal.pressed.connect(
            boost::bind( &World::hold, world)
        );
        
        // Pause
        controller->Start.signal.pressed.connect(
            boost::bind( &World::pause, world)
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

