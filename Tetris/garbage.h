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
#include <boost/signals2.hpp>
namespace bs2 = boost::signals2;

#include "block.h"
#include "constants.h"
#include "tetromino.h"
#include "timer.h"


class Garbage
{
public:
    Garbage(size_t width = WORLD_N_BLOCKS_X, size_t height = WORLD_N_BLOCKS_Y);
    ~Garbage();
    
    void draw();
    void update();
    
    bool checkCollision(Tetromino piece);
    int getFilledTSpinCorners(Tetromino piece);
    int addTetromino(Tetromino piece);// returns lineClears
    
    bool isClearing();
    int top();
    
private:
    std::vector<std::vector<Block*>> blocks;
    std::list<int> pendingClearLines;
    Timer lineClearTimer;
    bool _isClearing;
    
    int checkLineClears(std::list<int> rows);
    void markLinesForClearing(std::list<int> rows);
    void clearLines();

};

#endif /* defined(__Tetris__garbage__) */
