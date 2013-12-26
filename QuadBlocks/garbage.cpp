//
//  garbage.cpp
//  QuadBlocks
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
const double GAME_OVER_TIMING = 0.05;
const double CASCADE_TIMEOUT = 0.05;

const bool CASCADE_MODE = true;

Garbage::Garbage(size_t width, size_t height) :
    blocks(height, std::vector<Block*>(width, NULL)),
    _isClearing(false),
    _isCascading(false),
    cascadeStates(height, std::vector<int>(width, TBD)),
    _isGameOver(false),
    _gameOverRowIndex(height-1)
{
}

Garbage::~Garbage()
{
    deleteAllBlocks();
}

void Garbage::reset()
{
    _isClearing = false;
    _isCascading = false;
    _isGameOver = false;
    _gameOverRowIndex = blocks.size()-1;
    deleteAllBlocks();
}

void Garbage::draw()
{
    BOOST_FOREACH(std::vector<Block*> &blockRow, blocks) {
        BOOST_FOREACH(Block* &block, blockRow) {
            if (block) {
                block->draw();
            }
        }
    }
}

void Garbage::update()
{
    if ( gameOverTimer.isStarted() ) {
        if (gameOverTimer.getTime() > GAME_OVER_TIMING) {
            markNextGameOverLine();
        }
    }
    else if ( lineClearTimer.isStarted() ) {
        if (lineClearTimer.getTime() > LINE_CLEAR_TIMEOUT) {
            clearLines();
            if (CASCADE_MODE) {
//                if (!_isCascading) {
                    startCascade();
//                }
            }
        }
    }
}

void Garbage::pause()
{
    lineClearTimer.pause();
    cascadeTimer.pause();
}

void Garbage::unpause()
{
    lineClearTimer.unpause();
    cascadeTimer.unpause();
}

bool Garbage::isUpdating() {
    return ( lineClearTimer.isStarted()
             || gameOverTimer.isStarted() );
}

bool Garbage::isCascading() {
    return _isCascading;
}

void Garbage::gameOver()
{
    _isGameOver = true;
    gameOverTimer.start();
    markNextGameOverLine();
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
    //      [?] X [?]      [?]   [?]
    // e.g.     X  X        X  X  X
    //      [?] X [?] ,    [?] X [?] ,  etc.
    
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
            if ( (isValidGarbageCoord(x, y) && (blocks[y][x])) || (y < -1) ) {
                // either inside the garbage coords and there's a block there,
                // or it's the floor, which always counts for T-spins.
                // walls by themselves don't count.
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
    std::list<Block*> newBlocks;
    
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
                    newBlocks.push_back(blockCopy);
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
    
    addConnections(newBlocks);
    
    return checkLineClears(rowsWithNewBlocks);
}

void Garbage::startCascade()
{
    int nTBDs = WORLD_N_BLOCKS_Y * WORLD_N_BLOCKS_X;
    
    // clear cascade table and mark all spaces as TBD
    cascadeStates = std::vector< std::vector<int> >(WORLD_N_BLOCKS_Y, std::vector<int>(WORLD_N_BLOCKS_X, TBD));
    
    for (int y=0; y<WORLD_N_BLOCKS_Y; ++y) {
        for (int x=0; x<WORLD_N_BLOCKS_X; ++x) {
            if (cascadeStates[y][x] == TBD) {
                if (blocks[y][x] == NULL) {
                    cascadeStates[y][x] = EMPTY;
                    --nTBDs;
                }
                else if ( y <= 0 || cascadeStates[y-1][x] == STATIC) {
                    nTBDs -= cascadeMarkStatic(y, x);
                }
            }
        }
    }
    
    if (nTBDs > 0) {
        _isCascading = true;
        cascadeTimer.start();
    }
    else {
        _isCascading = false;
    }
}


int Garbage::doCascade()
{
    if ( cascadeTimer.isStarted() ) {
        if (cascadeTimer.getTime() <= CASCADE_TIMEOUT) {
            // wait
            return 0;
        }
        else {
            cascadeTimer.stop();
        }
    }
    
    // continue cascade
    int nTBDs = 0;
    for (int y=0; y<WORLD_N_BLOCKS_Y; ++y) {
        for (int x=0; x<WORLD_N_BLOCKS_X; ++x) {
            if (cascadeStates[y][x] == TBD) {
                if (y > 0 && cascadeStates[y-1][x] == EMPTY) {
                    // decrease block's Y position by 1
                    glm::vec2 pos = blocks[y][x]->position();
                    blocks[y][x]->setPosition(pos + glm::vec2(0,-1));

                    // shift block's pointer down by 1 index
                    if (blocks[y-1][x] != NULL) {
                        std::cerr << "Error: Cascading through an 'empty' position that somehow contains a block." << std::endl;
                        delete blocks[y-1][x];
                    }
                    blocks[y-1][x] = blocks[y][x];
                    blocks[y][x] = NULL;
                    
                    // swap EMPTY/TBD cascadeStates
                    cascadeStates[y-1][x] = TBD;
                    cascadeStates[y][x] = EMPTY;
                    
                    nTBDs++;
                }
                else {
                    cascadeMarkStatic(y, x);
                }
            }
        }
    }
    if (nTBDs > 0) {
        // blocks still falling
        cascadeTimer.start();
    }
    else {
        // blocks done falling. check for line clears
        std::list<int> allRows;
        for (int i=WORLD_N_BLOCKS_Y-1; i>=0; --i) {
            allRows.push_back(i);
        }
        if (!lineClearTimer.isStarted()) {
            int lineClears = checkLineClears(allRows);
            if (lineClears == 0) {
                // end cascade
                _isCascading = false;
            }
            return lineClears;
        }
    }
    return 0;
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
                removeConnections(block);
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

int Garbage::cascadeMarkStatic(int y, int x)
{
    int nMarked = 0;
    // mark position as static
    cascadeStates[y][x] = STATIC;
    ++nMarked;
    
    Block* block = blocks[y][x];
    if (block) {
        std::list<Block*> connectedList = connections.at(block);
        
        BOOST_FOREACH(Block* connectedBlock, connectedList) {
            glm::vec2 pos = connectedBlock->position() + glm::vec2(-0.5, -0.5);
            cascadeStates[pos.y][pos.x] = STATIC;
            ++nMarked;
        }
    }
    
    return nMarked;
}

void Garbage::addConnections(std::list<Block*> newBlocks)
{
    BOOST_FOREACH(Block* block, newBlocks) {
        std::list<Block*> connectionList;
        BOOST_FOREACH(Block* other, newBlocks) {
            if (other != block) {
                connectionList.push_back(other);
            }
        }
        connections[block] = connectionList;
    }
}

void Garbage::removeConnections(Block* block)
{
    // Find connected blocks
    std::list<Block*> connectionList = connections.at(block);
    
    // Erase this block from each connected block's list
    BOOST_FOREACH(Block* other, connectionList) {
        connections.at(other).remove(block);
    }
    // Delete this block's list
    connections.erase(block);
    
}

void Garbage::markNextGameOverLine()
{
    // Mark each block in the row
    BOOST_FOREACH(Block* &block, blocks[_gameOverRowIndex]) {
        if (block) {;
            block->setColor(BLOCK::COLOR::GRAY);
        }
    }
    
    // Prepare for next row
    gameOverTimer.stop();
    _gameOverRowIndex--;
    if (_gameOverRowIndex < 0) {
        _gameOverRowIndex = blocks.size()-1;
    }
    else {
        gameOverTimer.start();
    }
}

void Garbage::deleteAllBlocks()
{
    BOOST_FOREACH(std::vector<Block*> &blockRow, blocks) {
        BOOST_FOREACH(Block* &block, blockRow) {
            if (block) {
                delete block;
                block = NULL;
            }
        }
    }
}
