//
//  world.h
//  Tetris
//
//  Created by Cam Allen on 5/24/13.
//  Copyright (c) 2013 Cam Allen. All rights reserved.
//

#ifndef __Tetris__world__
#define __Tetris__world__

#include <list>
#include "block.h"
#include "camera.h"
#include "light.h"
#include "tetromino.h"


enum COMMAND {
    NONE,
    MOVE_LEFT,
    MOVE_RIGHT,
    ROTATE_CW,
    ROTATE_CCW,
    HOLD
};

class World
{
public:
    World();
    
    void update();
    void draw();
    
private:
//    Block b;
    Camera c;
    Tetromino piece;
    Light frontLight;
    Light topLight;
    
    std::list<Block> blocks;
    
};

#endif /* defined(__Tetris__world__) */
