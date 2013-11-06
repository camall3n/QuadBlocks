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
extern bool should_quit;

EventManager::EventManager() :
    controller(NULL),
    keyboard(NULL),
    menu(NULL),
    ui(NULL),
    world(NULL),
    paused(false)
{
    
}

void EventManager::setController(Controller *controller)
{
    this->controller = controller;
}

void EventManager::setKeyboard(Keyboard *keyboard)
{
    this->keyboard = keyboard;
}

void EventManager::setMenu(Menu *menu)
{
    this->menu = menu;
}

void EventManager::setUI(UI *ui)
{
    this->ui = ui;
}

void EventManager::setWorld(World *world)
{
    this->world = world;
}

void EventManager::activate()
{
    if (controller && keyboard && menu && ui && world) {
        UniversalSignals();
        GameMode(true);
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
        if (!menu) {
            std::cerr << "Error: Event Manager can't activate without a menu."
            << std::endl;
        }
        if (!ui) {
            std::cerr << "Error: Event Manager can't activate without a UI."
                      << std::endl;
        }
        if (!world) {
            std::cerr << "Error: Event Manager can't activate without a world."
            << std::endl;
        }
    }
}

void EventManager::UniversalSignals()
{
    // Pause
    controller->Start.signal.pressed.connect(
        boost::bind( &EventManager::TogglePause, this)
    );
    keyboard->Escape.signal.pressed.connect(
        boost::bind( &EventManager::TogglePause, this)
    );
    menu->signal.resume.connect(
        boost::bind( &EventManager::TogglePause, this)
    );

    
    menu->signal.toggleDevMode.connect(
        boost::bind( &World::toggleDevMode, world)
    );
    menu->signal.toggleDevMode.connect(
        boost::bind( &UI::ToggleDevMode, ui)
    );
    
    menu->signal.exit.connect(
        boost::bind( &EventManager::Exit, this)
    );
}

void EventManager::GameMode(bool activate)
{
    if (activate) {
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
        keyboard->ArrowRight.signal.pressed.connect(
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
        keyboard->ArrowLeft.signal.pressed.connect(
            boost::bind( &World::queueMoveLeft, world)
        );

        
        // Rotate CW
        controller->RT.signal.pressed.connect(
            boost::bind( &World::queueRotateCW, world)
        );
        keyboard->Period.signal.pressed.connect(
            boost::bind( &World::queueRotateCW, world)
        );
        keyboard->X.signal.pressed.connect(
            boost::bind( &World::queueRotateCW, world)
        );

        
        // Rotate CCW
        controller->LT.signal.pressed.connect(
            boost::bind( &World::queueRotateCCW, world)
        );
        keyboard->Comma.signal.pressed.connect(
            boost::bind( &World::queueRotateCCW, world)
        );
        keyboard->Z.signal.pressed.connect(
            boost::bind( &World::queueRotateCCW, world)
        );
        
        // Hard Drop
        controller->A.signal.pressed.connect(
            boost::bind( &World::queueHardDrop, world)
        );
        controller->Up.signal.pressed.connect(
            boost::bind( &World::queueHardDrop, world)
        );
        keyboard->Return.signal.pressed.connect(
            boost::bind( &World::queueHardDrop, world)
        );


        // Soft Drop
        controller->LS.signal.movedDown.connect(
            boost::bind( &World::queueSoftDrop, world)
        );
        controller->Down.signal.pressed.connect(
            boost::bind( &World::queueSoftDrop, world)
        );
        keyboard->S.signal.pressed.connect(
            boost::bind( &World::queueSoftDrop, world)
        );
        keyboard->ArrowDown.signal.pressed.connect(
            boost::bind( &World::queueSoftDrop, world)
        );

        
        // Hold
        controller->Y.signal.pressed.connect(
            boost::bind( &World::queueHold, world)
        );
        keyboard->Space.signal.pressed.connect(
            boost::bind( &World::queueHold, world)
        );
        
        
        // UI signals
        world->signal.newPoints.connect(
            boost::bind( &UI::SetNewPoints, ui, _1)
        );
        world->signal.scoreChanged.connect(
            boost::bind( &UI::SetScore, ui, _1)
        );
        world->signal.linesLeftChanged.connect(
            boost::bind( &UI::SetLines, ui, _1)
        );
        world->signal.levelChanged.connect(
            boost::bind( &UI::SetLevel, ui, _1)
        );
        world->signal.allClear.connect(
            boost::bind( &UI::DisplayAllClear, ui)
        );
        world->signal.lineClear.connect(
            boost::bind( &UI::DisplayLineClear, ui, _1)
        );
        world->signal.tSpin.connect(
            boost::bind( &UI::DisplayTSpin, ui, _1, _2)
        );
    }
    else {
        // Move Right
        controller->LS.signal.movedRight.disconnect_all_slots();
        controller->RB.signal.pressed.disconnect_all_slots();
        controller->Right.signal.pressed.disconnect_all_slots();
        keyboard->D.signal.pressed.disconnect_all_slots();
        keyboard->ArrowRight.signal.pressed.disconnect_all_slots();
        
        // Move Left
        controller->LS.signal.movedLeft.disconnect_all_slots();
        controller->LB.signal.pressed.disconnect_all_slots();
        controller->Left.signal.pressed.disconnect_all_slots();
        keyboard->A.signal.pressed.disconnect_all_slots();
        keyboard->ArrowLeft.signal.pressed.disconnect_all_slots();
        
        // Rotate CW
        controller->RT.signal.pressed.disconnect_all_slots();
        keyboard->Period.signal.pressed.disconnect_all_slots();
        keyboard->X.signal.pressed.disconnect_all_slots();
        
        // Rotate CCW
        controller->LT.signal.pressed.disconnect_all_slots();
        keyboard->Comma.signal.pressed.disconnect_all_slots();
        keyboard->Z.signal.pressed.disconnect_all_slots();
        
        // Hard Drop
        controller->A.signal.pressed.disconnect_all_slots();
        controller->Up.signal.pressed.disconnect_all_slots();
        keyboard->Return.signal.pressed.disconnect_all_slots();

        // Soft Drop
        controller->LS.signal.movedDown.disconnect_all_slots();
        controller->Down.signal.pressed.disconnect_all_slots();
        keyboard->S.signal.pressed.disconnect_all_slots();
        keyboard->ArrowDown.signal.pressed.disconnect_all_slots();
        
        // Hold
        controller->Y.signal.pressed.disconnect_all_slots();
        keyboard->Space.signal.pressed.disconnect_all_slots();
        
        // Pause
        controller->Start.signal.pressed.connect(
            boost::bind( &EventManager::TogglePause, this)
        );
        keyboard->Backquote.signal.pressed.connect(
            boost::bind( &EventManager::TogglePause, this)
        );
        
        // UI signals
        world->signal.newPoints.disconnect_all_slots();
        world->signal.scoreChanged.disconnect_all_slots();
        world->signal.linesLeftChanged.disconnect_all_slots();
        world->signal.levelChanged.disconnect_all_slots();
        world->signal.allClear.disconnect_all_slots();
        world->signal.lineClear.disconnect_all_slots();
        world->signal.tSpin.disconnect_all_slots();
    }
}

void EventManager::MenuMode(bool activate)
{
    static bs2::connection c0;
    static bs2::connection c1;
    static bs2::connection c2;
    static bs2::connection c3;
    static bs2::connection c4;
    static bs2::connection c5;
    
    if (activate) {
        c0 = keyboard->W.signal.pressed.connect(
            boost::bind( &UI::SelectPrevMenuItem, ui)
        );
        c1 = keyboard->ArrowUp.signal.pressed.connect(
            boost::bind( &UI::SelectPrevMenuItem, ui)
        );
        c2 = keyboard->S.signal.pressed.connect(
            boost::bind( &UI::SelectNextMenuItem, ui)
        );
        c3 = keyboard->ArrowDown.signal.pressed.connect(
            boost::bind( &UI::SelectNextMenuItem, ui)
        );
        c4 = keyboard->Return.signal.pressed.connect(
            boost::bind( &UI::ClickMenuItem, ui)
        );
        c5 = keyboard->Return.signal.pressed.connect(
            boost::bind( &Menu::Select, menu)
        );
    }
    else {
        c0.disconnect();
        c1.disconnect();
        c2.disconnect();
        c3.disconnect();
        c4.disconnect();
        c5.disconnect();
    }
}


void EventManager::LSReallyMovedUp()
{
    if (controller->LS.y() > controller->LS.x()) {
        world->queueHardDrop();
    }
}


void EventManager::TogglePause()
{
    if (!paused) {
        paused = true;
//        GameMode(false);
        MenuMode(true);
    }
    else {
        paused = false;
        MenuMode(false);
//        GameMode(true);
    }
    world->togglePause();
    ui->TogglePause();
}

void EventManager::Exit()
{
    should_quit = true;
}