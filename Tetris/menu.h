//
//  menu.h
//  Tetris
//
//  Created by Cam Allen on 7/17/13.
//  Copyright (c) 2013 Cam Allen. All rights reserved.
//

#ifndef __Tetris__menu__
#define __Tetris__menu__

#include "ui.h"
#include <boost/signals2.hpp>
namespace bs2 = boost::signals2;

class Menu
{
    UI* ui;
    
    enum MENU_STATE {
        MAIN,
        CONTROLS,
        SOUNDS,
        DEVELOPER
    };
    MENU_STATE state;
    
public:
    Menu();
    
    void SetUI(UI* ui);
    void Select();
    
    struct signalList {
        bs2::signal<void ()> resume;
        bs2::signal<void ()> exit;
        bs2::signal<void ()> toggleDevMode;
    } signal;
    
private:
    void Resume();
    void Exit();
    void DevMode();
};

#endif /* defined(__Tetris__menu__) */
