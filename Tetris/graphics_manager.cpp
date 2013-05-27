//
//  graphics_manager.cpp
//  Tetris
//
//  Created by Cam Allen on 5/11/13.
//  Copyright (c) 2013 Cam Allen. All rights reserved.
//

#include "graphics_manager.h"

#include "block.h"

GraphicsManager::GraphicsManager()
{
    Block b;
    RegisterModel(b);
}

void GraphicsManager::RegisterModel(Model m)
{
    m._baseProgram = 5;
}