//
//  tetromino.h
//  Tetris
//
//  Created by Cam Allen on 4/14/13.
//  Copyright (c) 2013 Cam Allen. All rights reserved.
//

#ifndef __Tetris__tetromino__
#define __Tetris__tetromino__

#include <glm/glm.hpp>
#include <iostream>
#include <list>
#include <vector>
#include "block.h"

namespace TETROMINO {
    enum TYPE {
        I = 0,
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
    Tetromino(TETROMINO::TYPE type=TETROMINO::I);
//    ~Tetromino();
    
    void draw();
    void update(TETROMINO::COMMAND command);
    
    void setPosition(glm::vec2 pos);
    void setRotation(float angle);

private:
    TETROMINO::TYPE _type;
    TETROMINO::STATE _state;
    TETROMINO::COMMAND _currentCommand;

    glm::vec4 _color;
    glm::vec2 _position;
    glm::vec2 _center;
    float _rotation;
    
    std::list<Block> _blocksList;
    
    static glm::vec4 getColor(TETROMINO::TYPE type);
    static glm::vec2 getCenter(TETROMINO::TYPE type);
    static std::vector<glm::vec2> getBaseOffsets(TETROMINO::TYPE type);
};

#endif /* defined(__Tetris__tetromino__) */
