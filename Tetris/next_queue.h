//
//  next_queue.h
//  Tetris
//
//  Created by Cam Allen on 6/7/13.
//  Copyright (c) 2013 Cam Allen. All rights reserved.
//

#ifndef __Tetris__next_queue__
#define __Tetris__next_queue__

#include <queue>
#include "tetromino.h"

class NextQueue
{
public:
    NextQueue();
    
    Tetromino getNext();
    
private:
    std::queue<Tetromino> current;
    std::queue<Tetromino> next;
    
    std::vector<TETROMINO::TYPE> typeVec;
    
};

#endif /* defined(__Tetris__next_queue__) */
