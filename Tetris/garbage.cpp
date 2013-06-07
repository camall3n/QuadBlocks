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
            if ( randint(0, 5) ) {
                Block* b = new Block(BLOCK::COLOR::NUMBER[randint(0, BLOCK::COLOR::MAX_NUMBER-1)]);
                b->setPosition(glm::vec3(i+0.5, j+0.5, 0));
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

static bool isValidGarbageCoord(int x, int y)
{
    if ((x >= 0 && x < WORLD_N_BLOCKS_X) &&
        (y >= 0 && y < WORLD_N_BLOCKS_Y))
        return true;
    return false;
}

bool Garbage::checkCollision(Tetromino piece)
{
    glm::vec2 basePosition = piece.position();
    glm::mat4 square = piece.collisionSquare();
    
    for (int i=0; i<piece.collisionSquareSize(); i++) {
        for (int j=0; j<piece.collisionSquareSize(); j++) {
            if (square[i][j]) {
                glm::vec2 position = basePosition + glm::vec2(j+0.5, i+0.5);
                int x = round(position.x)-1;
                int y = round(position.y)-1;
                
                if ( isValidGarbageCoord(x, y) && (blocks[y][x]) ) {
                    // There's a block at those coordinates
                    return true;
                }
            }
        }
    }
    
    return false;
}

void Garbage::addTetromino(Tetromino piece)
{
    glm::vec2 basePos = piece.position();
    glm::mat4 square = piece.collisionSquare();
    Block refBlock = piece.blocks().front();
    
    for (int i=0; i<piece.collisionSquareSize(); i++) {
        for (int j=0; j<piece.collisionSquareSize(); j++) {
            if (square[i][j]) {
                glm::vec2 blockPos = basePos + glm::vec2(j, i);
                int row = round(blockPos.y);
                int col = round(blockPos.x);
                
                if (row < WORLD_N_BLOCKS_Y) {
                    if (blocks[row][col]) {
                        std::cerr << "Error: Added piece to garbage that overlapped a garbage block." << std::endl;
                        delete blocks[row][col];
                    }

                    Block* blockCopy = new Block(refBlock);
                    blockCopy->setPosition(glm::vec3(blockPos.x+0.5, blockPos.y+0.5, 0));
                    blockCopy->setOffset(glm::vec2(0,0));
                    blockCopy->setRotation(0);
                    blocks[row][col] = blockCopy;
                }
                else {
                    // top-out!!
                }
            }
        }
    }
    
//    checkLineClears();
    
}

