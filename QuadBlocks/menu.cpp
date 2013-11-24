//
//  menu.cpp
//  QuadBlocks
//
//  Created by Cam Allen on 7/17/13.
//  Copyright (c) 2013 Cam Allen. All rights reserved.
//

#include "menu.h"
#include <iostream>

Menu::Menu() :
state(MAIN),
ui(NULL)
{ }

void Menu::SetUI(UI* ui)
{
    this->ui = ui;
}

void Menu::Select()
{
    if (!ui) {
        std::cerr << "[Menu] Error: No UI set. UI = NULL" << std::endl;
        return;
    }
    switch (state)
    {
        case MAIN:
            switch(ui->GetSelection())
        {
            case 0:
                Start();
                break;
            case 1:
                state = CONTROLS;
                break;
            case 2:
                state = SOUNDS;
                break;
            case 3:
                signal.toggleDevMode();
                
                break;
            case 4:
                Exit();
                break;
            default:
                break;
        }
            break;
        
        case PAUSE:
            switch(ui->GetSelection())
            {
                case 0:
                    Resume();
                    break;
                case 1:
                    state = CONTROLS;
                    break;
                case 2:
                    state = SOUNDS;
                    break;
                case 3:
                    signal.toggleDevMode();
                    
                    break;
                case 4:
                    Exit();
                    break;
                default:
                    break;
            }
            break;
            
        default:
            state = MAIN;
            break;
    }
}

void Menu::MainMenu()
{
    state = MAIN;
}

void Menu::Resume()
{
    signal.resume();
}

void Menu::Start()
{
    state = PAUSE;
    signal.start();
}

void Menu::Exit()
{
    signal.exit();
}

void Menu::DevMode()
{
    signal.toggleDevMode();
}