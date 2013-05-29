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

class World
{
public:
    World();
    
    void draw();
    
private:
//    Block b;
    Camera c;
    Tetromino piece;
    Light light;
    
    std::list<Block> blocks;
    
};

#endif /* defined(__Tetris__world__) */
