//
//  tetromino.h
//  Tetris
//
//  Created by Cam Allen on 4/14/13.
//  Copyright (c) 2013 Cam Allen. All rights reserved.
//

#ifndef __Tetris__tetromino__
#define __Tetris__tetromino__

#include <iostream>
#include <list>
#include <glm/glm.hpp>
#include "block.h"

namespace TETROMINO {
    enum TYPE {
        I,
        O,
        T,
        S,
        Z,
        J,
        L,
    };
    
    enum STATE {
        ACTIVE,
        HOLDING,
        ON_DECK,
        UPCOMING,
        GHOST
    };
    
    enum COMMAND {
        NONE,
        MOVE_LEFT,
        MOVE_RIGHT,
        ROTATE_CW,
        ROTATE_CCW,
        SOFT_DROP,
        HARD_DROP,
        HOLD,
        UNHOLD
    };
};


class Tetromino
{
public:
    Tetromino();
    Tetromino(int type);
    ~Tetromino();
    
    void draw();
    void update(TETROMINO::COMMAND command);
    
private:
    TETROMINO::TYPE _type;
    TETROMINO::STATE _state;
    TETROMINO::COMMAND _currentCommand;

    glm::vec2 _position;
    float _rotation;
    
    std::list<Block> _blocksList;
    
    Tetromino* parent;
    Tetromino* ghost;
};

#endif /* defined(__Tetris__tetromino__) */
