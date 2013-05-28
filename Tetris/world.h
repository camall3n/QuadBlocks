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

class World
{
public:
    World();
    
    void draw();
    
private:
//    Block b;
    Camera c;
    Light light;
    
    std::list<Block> blocks;
    
};

#endif /* defined(__Tetris__world__) */
