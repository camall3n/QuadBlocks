//
//  garbage.h
//  Tetris
//
//  Created by Cam Allen on 6/1/13.
//  Copyright (c) 2013 Cam Allen. All rights reserved.
//

#ifndef __Tetris__garbage__
#define __Tetris__garbage__

#include <vector>
#include "block.h"
#include "constants.h"
#include "tetromino.h"

class Garbage
{
public:
    Garbage(size_t width = WORLD_N_BLOCKS_X, size_t height = WORLD_N_BLOCKS_Y);
    ~Garbage();
    
    void draw();
    
    bool checkCollision(Tetromino piece);
    void addTetromino(Tetromino piece);
    
private:
    std::vector<std::vector<Block*>> blocks;
};

#endif /* defined(__Tetris__garbage__) */
