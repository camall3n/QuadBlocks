//
//  next_queue.h
//  QuadBlocks
//
//  Created by Cam Allen on 6/7/13.
//  Copyright (c) 2013 Cam Allen. All rights reserved.
//

#ifndef __QuadBlocks__next_queue__
#define __QuadBlocks__next_queue__

#include <list>
#include "tetromino.h"

class NextQueue
{
public:
    NextQueue();
    
    void reset();
    
    void draw();
    void update();
    
    Tetromino getNext();
    
private:
    std::list<Tetromino> current;
    std::list<Tetromino> next;
    
    std::vector<TETROMINO::TYPE> typeVec;
    
    void arrangePieces();
    
};

#endif /* defined(__QuadBlocks__next_queue__) */
