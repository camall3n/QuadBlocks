//
//  garbage.cpp
//  Tetris
//
//  Created by Cam Allen on 6/1/13.
//  Copyright (c) 2013 Cam Allen. All rights reserved.
//

#include "garbage.h"

#include <boost/foreach.hpp>
#include "constants.h"
#include "utility.h"

Garbage::Garbage(size_t width, size_t height) :
    blocks(height, std::vector<Block*>(width, NULL))
{
    for (int j=0; j<height/3; j++) {
        for (int i=0; i<width; i++) {
            if ( randint(0, 6) ) {
                Block* b = new Block(BLOCK::COLOR::NUMBER[randint(0, BLOCK::COLOR::MAX_NUMBER)]);
                b->setPosition(glm::vec3(i+0.5, j+0.5,-0.2));
                blocks[j][i] = b;
            }
        }
    }
}

Garbage::~Garbage()
{
    BOOST_FOREACH(std::vector<Block*> blockRow, blocks) {
        BOOST_FOREACH(Block* block, blockRow) {
            if (block) {
                delete block;
            }
        }
    }
}

void Garbage::draw()
{
    BOOST_FOREACH(std::vector<Block*> blockRow, blocks) {
        BOOST_FOREACH(Block* block, blockRow) {
            if (block) {
                block->draw();
            }
        }
    }
}