//
//  well.h
//  Tetris
//
//  Created by Cam Allen on 6/1/13.
//  Copyright (c) 2013 Cam Allen. All rights reserved.
//

#ifndef __Tetris__well__
#define __Tetris__well__

#include <list>
#include "block.h"
#include "constants.h"
#include "light.h"
#include "tetromino.h"

class Well
{
public:
    Well(size_t width = WORLD_N_BLOCKS_X, size_t height = WORLD_N_BLOCKS_Y);
    
    void draw();
    
    bool checkCollision(Tetromino piece);
    
    void setFrontLight(Light light);
    void setTopLight(Light light);
    
private:
    Light frontLight;
    Light topLight;
    
    std::list<Block> border;
    std::list<Block> backWall;
};

#endif /* defined(__Tetris__well__) */
