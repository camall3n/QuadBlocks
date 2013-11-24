//
//  tetromino.h
//  QuadBlocks
//
//  Created by Cam Allen on 4/14/13.
//  Copyright (c) 2013 Cam Allen. All rights reserved.
//

#ifndef __QuadBlocks__tetromino__
#define __QuadBlocks__tetromino__

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
    bool operator==(const Tetromino& that);
    bool operator!=(const Tetromino& that);
    
    void draw();
    
    glm::vec2 position();
    float rotation();
    TETROMINO::TYPE type();
    void setPosition(glm::vec2 pos);
    void resetPosition();
    void holdPosition();
    void setRotation(float angle);// range: [0, 1)
    void setState(TETROMINO::STATE state);
    
    std::list<Block> blocks();
    
    glm::mat4 collisionSquare();// lower-left is mat[0][0]
    int collisionSquareSize();// number of blocks on a side
    
private:
    TETROMINO::TYPE _type;
    TETROMINO::STATE _state;

    glm::vec3 _color;
    glm::vec2 _position;
    glm::vec2 _center;
    float _rotation;// range: [-0.5, 0.5)
    
    std::list<Block> _blocks;
    std::vector<glm::vec2> _baseOffsets;
    
    static glm::vec3 getColor(TETROMINO::TYPE type);
    static glm::vec2 getCenter(TETROMINO::TYPE type);
    static std::vector<glm::vec2> getBaseOffsets(TETROMINO::TYPE type);
    
    float getMaxAbsOffset();
};

#endif /* defined(__QuadBlocks__tetromino__) */
