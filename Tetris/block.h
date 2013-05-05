//
//  block.h
//  Tetris
//
//  Created by Cam Allen on 4/14/13.
//  Copyright (c) 2013 Cam Allen. All rights reserved.
//

#ifndef __Tetris__block__
#define __Tetris__block__

#include <iostream>
#include <glm/glm.hpp>

namespace BLOCK {
    namespace COLOR {
        const glm::vec4 CYAN   = glm::vec4(0.f, .9f, .9f, 1.f);
        const glm::vec4 YELLOW = glm::vec4(1.f, .9f, 0.f, 1.f);
        const glm::vec4 PURPLE = glm::vec4(.7f, 0.f, 1.f, 1.f);
        const glm::vec4 GREEN  = glm::vec4(0.f, .9f, 0.f, 1.f);
        const glm::vec4 RED    = glm::vec4(.9f, 0.f, 0.f, 1.f);
        const glm::vec4 BLUE   = glm::vec4(0.f, 0.f, 1.f, 1.f);
        const glm::vec4 ORANGE = glm::vec4(1.f, .5f, 1.f, 1.f);
        const glm::vec4 GRAY   = glm::vec4(.7f, .7f, .7f, 1.f);
        const glm::vec4 WHITE  = glm::vec4(1.f, 1.f, 1.f, 1.f);
    };
    
    enum STATE {
        USER_CONTROL,
        GHOST,
        FALLING,
        FIXED,
        CLEARING,
    };
};

class Block
{
public:
    Block();
    Block(int color);
    ~Block();
    
    void draw();
    
private:
    BLOCK::STATE _state;
    
    glm::vec2 _position;
    glm::vec2 _offset;
    float _rotation;
    
    glm::vec4 _color;
    
};

#endif /* defined(__Tetris__block__) */
