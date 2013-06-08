//
//  next_queue.h
//  Tetris
//
//  Created by Cam Allen on 6/7/13.
//  Copyright (c) 2013 Cam Allen. All rights reserved.
//

#ifndef __Tetris__next_queue__
#define __Tetris__next_queue__

#include <list>
#include "tetromino.h"

class NextQueue
{
public:
    NextQueue();
    
    void draw();
    void update();
    
    Tetromino getNext();
    
private:
    std::list<Tetromino> current;
    std::list<Tetromino> next;
    
    std::vector<TETROMINO::TYPE> typeVec;
    
    void arrangePieces();
    
};

#endif /* defined(__Tetris__next_queue__) */
