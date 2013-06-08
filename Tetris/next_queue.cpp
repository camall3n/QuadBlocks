//
//  next_queue.cpp
//  Tetris
//
//  Created by Cam Allen on 6/7/13.
//  Copyright (c) 2013 Cam Allen. All rights reserved.
//

#include "next_queue.h"

#include <algorithm>
#include <boost/foreach.hpp>
#include <vector>
#include "tetromino.h"

int myrandom(int i) { return ::random() % i; }

NextQueue::NextQueue() :
    typeVec(7)
{
    for (int i=0; i<7; i++) {
        typeVec[i] = (TETROMINO::TYPE) i;
    }
    
    std::random_shuffle(typeVec.begin(), typeVec.end(), myrandom);
    BOOST_FOREACH(TETROMINO::TYPE type, typeVec) {
        Tetromino piece(type);
        current.push(piece);
    }
    
    std::random_shuffle(typeVec.begin(), typeVec.end(), myrandom);
    BOOST_FOREACH(TETROMINO::TYPE type, typeVec) {
        Tetromino piece(type);
        next.push(piece);
    }
}

Tetromino NextQueue::getNext()
{
    Tetromino piece = current.front();
    current.pop();
    
    current.push(next.front());
    next.pop();
    
    if (next.empty()) {
        std::random_shuffle(typeVec.begin(), typeVec.end(), myrandom);
        BOOST_FOREACH(TETROMINO::TYPE type, typeVec) {
            Tetromino piece(type);
            next.push(piece);
        }
    }
    
    return piece;
}