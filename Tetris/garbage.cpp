//
//  garbage.cpp
//  Tetris
//
//  Created by Cam Allen on 6/1/13.
//  Copyright (c) 2013 Cam Allen. All rights reserved.
//

#include "garbage.h"

#include <boost/foreach.hpp>
#include <functional>
#include "constants.h"
#include "utility.h"

const double LINE_CLEAR_TIMEOUT = 0.25;

Garbage::Garbage(size_t width, size_t height) :
    blocks(height, std::vector<Block*>(width, NULL)),
    _isClearing(false)
{
//    for (int j=0; j<height/3; j++) {
//        for (int i=0; i<width; i++) {
//            if ( randint(0, 5) ) {
//                Block* b = new Block(BLOCK::COLOR::NUMBER[randint(0, BLOCK::COLOR::MAX_NUMBER-1)]);
//                b->setPosition(glm::vec3(i+0.5, j+0.5, 0));
//                blocks[j][i] = b;
//            }
//        }
//    }
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

void Garbage::update()
{
    if ( lineClearTimer.isStarted() ) {
        if (lineClearTimer.getTime() > LINE_CLEAR_TIMEOUT) {
            clearLines();
        }
    }
    else {
        std::cerr << "Warning: Garbage::update() called when no lines were clearing." << std::endl;
    }
}

bool Garbage::isClearing()
{
    return _isClearing;
}

int Garbage::top()
{
    int top = 0;
    bool foundBlock = false;
    int row = 1;
    BOOST_FOREACH(std::vector<Block*> blockRow, blocks) {
        foundBlock = false;
        BOOST_FOREACH(Block* block, blockRow) {
            if (block) {
                foundBlock = true;
                break;
            }
        }
        if (foundBlock) {
            top = row;
        }
        row++;
    }
    return top;
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

int Garbage::getFilledTSpinCorners(Tetromino piece)
{
    //    [?][ ][?]
    //    [ ] X [ ]
    //    [?][ ][?]
    
    if (piece.type() != TETROMINO::T) {
        return 0;
    }

    int nFilledCorners = 0;
    
    glm::vec2 basePosition = piece.position();
    
    for (int i=0; i<piece.collisionSquareSize(); i+=2) {
        for (int j=0; j<piece.collisionSquareSize(); j+=2) {
            glm::vec2 position = basePosition + glm::vec2(j+0.5, i+0.5);
            int x = round(position.x)-1;
            int y = round(position.y)-1;
            if ( !isValidGarbageCoord(x, y) || (blocks[y][x]) ) {
                // it's either outside the garbage coords (aka wall / floor)
                // or, it's inside, and there's a block there
                nFilledCorners++;
            }
        }
    }

    return nFilledCorners;
}

int Garbage::addTetromino(Tetromino piece)
{
    glm::vec2 basePos = piece.position();
    glm::mat4 square = piece.collisionSquare();
    Block refBlock = piece.blocks().front();
    
    std::list<int> rowsWithNewBlocks;
    
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
                    if (rowsWithNewBlocks.empty() || (rowsWithNewBlocks.back() < row))
                    {
                        rowsWithNewBlocks.push_back(row);
                    }
                }
                else {
                    // top-out!!
                    return -1;
                }
            }
        }
    }
    
    return checkLineClears(rowsWithNewBlocks);
}

int Garbage::checkLineClears(std::list<int> rows)
{
    // Put highest numbered row at the beginning
    rows.sort(std::greater<int>());
    
    std::list<int> linesToClear;
    
    BOOST_FOREACH(int row, rows) {
        int nBlocksInRow = 0;
        BOOST_FOREACH(Block* block, blocks[row]) {
            if (block) {
                nBlocksInRow++;
            }
        }
        if (nBlocksInRow == WORLD_N_BLOCKS_X) {
            linesToClear.push_back(row);
        }
    }
    
    if (!linesToClear.empty()) {
        markLinesForClearing(linesToClear);
        return linesToClear.size();
    }
    
    return 0;
}

void Garbage::markLinesForClearing(std::list<int> rows)
{
    if (rows.empty()) {
        return;
    }
    
    BOOST_FOREACH(int row, rows) {
        BOOST_FOREACH(Block* block, blocks[row]) {
            if (block) {
                block->setColor(BLOCK::COLOR::WHITE);
            }
            else {
                std::cerr << "Error: Marking block for clearing that does not exist." << std::endl;
            }
        }
    }
    
    _isClearing = true;
    pendingClearLines = rows;
    lineClearTimer.start();
}

void Garbage::clearLines()
{
    BOOST_FOREACH(int row, pendingClearLines) {
        
        // Delete blocks in that row
        BOOST_FOREACH(Block* &block, blocks[row]) {
            if (block) {
                delete block;
                block = NULL;
            }
            else {
                std::cerr << "Error: Clearing block that does not exist." << std::endl;
            }
        }
        
        // Shift higher rows down
        for (int j=row+1; j<WORLD_N_BLOCKS_Y; j++) {
            for (int i=0; i<WORLD_N_BLOCKS_X; i++) {
                if ( blocks[j][i] ) {
                    glm::vec2 pos = blocks[j][i]->position();
                    blocks[j][i]->setPosition(pos+glm::vec2(0,-1));
                    blocks[j-1][i] = blocks[j][i];
                    blocks[j][i] = NULL;
                }
            }
        }
    }
    
    pendingClearLines.clear();
    lineClearTimer.stop();
    _isClearing = false;
}

