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
};


class Tetromino
{
public:
    Tetromino(TETROMINO::TYPE type=TETROMINO::I);
//    Tetromino(const Tetromino& that);
//    Tetromino& operator=(const Tetromino& that);
//    ~Tetromino();
    
    void draw();
    
    glm::vec2 position();
    void setPosition(glm::vec2 pos);
    void setRotation(float angle);// range [0,1)
    
    
    glm::mat4 collisionSquare();// bottom- and left-justified
    int collisionSquareSize();// number of blocks on a side
    
private:
    TETROMINO::TYPE _type;
    TETROMINO::STATE _state;

    glm::vec4 _color;
    glm::vec2 _position;
    glm::vec2 _center;
    float _rotation;// range [0,1)
    
    std::list<Block> _blocks;
    static std::vector<glm::vec2> _baseOffsets;
    
    static glm::vec4 getColor(TETROMINO::TYPE type);
    static glm::vec2 getCenter(TETROMINO::TYPE type);
    static std::vector<glm::vec2> getBaseOffsets(TETROMINO::TYPE type);
    
    float getMaxAbsOffset();
};

#endif /* defined(__Tetris__tetromino__) */
