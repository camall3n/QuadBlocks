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
    keyboard(NULL),
    world(NULL),
    ui(NULL)
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

void EventManager::setUI(UI *ui)
{
    this->ui = ui;
}

void EventManager::setKeyboard(Keyboard *keyboard)
{
    this->keyboard = keyboard;
}

void EventManager::activate()
{
    if (controller && world && ui && keyboard) {
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
        keyboard->D.signal.pressed.connect(
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
        keyboard->A.signal.pressed.connect(
            boost::bind( &World::queueMoveLeft, world)
        );

        
        // Rotate CW
        controller->RT.signal.pressed.connect(
            boost::bind( &World::queueRotateCW, world)
        );
        keyboard->Period.signal.pressed.connect(
            boost::bind( &World::queueRotateCW, world)
        );

        
        // Rotate CCW
        controller->LT.signal.pressed.connect(
            boost::bind( &World::queueRotateCCW, world)
        );
        keyboard->Comma.signal.pressed.connect(
            boost::bind( &World::queueRotateCCW, world)
        );
        
        // Hard Drop
        controller->A.signal.pressed.connect(
            boost::bind( &World::queueHardDrop, world)
        );
        keyboard->Return.signal.pressed.connect(
            boost::bind( &World::queueHardDrop, world)
        );


        // Soft Drop
        controller->LS.signal.movedDown.connect(
            boost::bind( &World::queueSoftDrop, world)
        );
        keyboard->S.signal.pressed.connect(
            boost::bind( &World::queueSoftDrop, world)
        );

        
        // Hold
        controller->Y.signal.pressed.connect(
            boost::bind( &World::queueHold, world)
        );
        keyboard->Space.signal.pressed.connect(
            boost::bind( &World::queueHold, world)
        );

        
        // Pause
        controller->Start.signal.pressed.connect(
            boost::bind( &World::togglePause, world)
        );
        keyboard->Backquote.signal.pressed.connect(
            boost::bind( &World::togglePause, world)
        );
        
        
        // UI signals
        world->signal.scoreChanged.connect(
            boost::bind( &UI::SetScore, ui, _1)
        );
        world->signal.linesLeftChanged.connect(
            boost::bind( &UI::SetLines, ui, _1)
        );
        world->signal.levelChanged.connect(
            boost::bind( &UI::SetLevel, ui, _1)
        );

    }
    else {
        if (!controller) {
            std::cerr << "Error: Event Manager can't activate without a controller."
                      << std::endl;
        }
        if (!keyboard) {
            std::cerr << "Error: Event Manager can't activate without a keyboard."
                      << std::endl;
        }
        if (!world) {
            std::cerr << "Error: Event Manager can't activate without a world."
                      << std::endl;
        }
        if (!ui) {
            std::cerr << "Error: Event Manager can't activate without a UI."
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
