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
#include "constants.h"
#include "tetromino.h"

extern const int WORLD_N_BLOCKS_X;
extern const int WORLD_N_BLOCKS_Y;

int myrandom(int i) { return ::rand() % i; }

NextQueue::NextQueue() :
    typeVec(7)
{
    reset();
}

void NextQueue::reset()
{
    while (!current.empty()) {
        current.pop_front();
    }
    while (!next.empty()) {
        next.pop_front();
    }
    
    srand ( (unsigned int) time(NULL) );
    int type0 = rand() % 7;
    for (int i=0; i<7; i++) {
        typeVec[i] = (TETROMINO::TYPE) ((type0+i) % 7);
    }
    
    std::random_shuffle(typeVec.begin(), typeVec.end(), myrandom);
    BOOST_FOREACH(TETROMINO::TYPE type, typeVec) {
        Tetromino piece(type);
        current.push_back(piece);
    }
    arrangePieces();
    
    std::random_shuffle(typeVec.begin(), typeVec.end(), myrandom);
    BOOST_FOREACH(TETROMINO::TYPE type, typeVec) {
        Tetromino piece(type);
        next.push_back(piece);
    }
}

void NextQueue::draw()
{
    int i=0;
    BOOST_FOREACH(Tetromino &piece, current) {
        if (i<5) {
            piece.draw();
        }
        i++;
    }
}

void NextQueue::update()
{
    
}

Tetromino NextQueue::getNext()
{
    Tetromino piece = current.front();
    current.pop_front();
    
    current.push_back(next.front());
    next.pop_front();
    
    arrangePieces();
    
    if (next.empty()) {
        std::random_shuffle(typeVec.begin(), typeVec.end(), myrandom);
        BOOST_FOREACH(TETROMINO::TYPE type, typeVec) {
            Tetromino piece(type);
            next.push_back(piece);
        }
    }
    
    return piece;
}


void NextQueue::arrangePieces()
{
    int i=0;
    BOOST_FOREACH(Tetromino &piece, current) {
        glm::vec2 pos(WORLD_N_BLOCKS_X + 2, WORLD_N_BLOCKS_Y - 3*(i+1) );
        if (piece.type() == TETROMINO::O) {
            pos += glm::vec2(0,1);
        }
        piece.setPosition(pos);
        i++;
    }
}