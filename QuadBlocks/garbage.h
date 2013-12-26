//
//  garbage.h
//  QuadBlocks
//
//  Created by Cam Allen on 6/1/13.
//  Copyright (c) 2013 Cam Allen. All rights reserved.
//

#ifndef __QuadBlocks__garbage__
#define __QuadBlocks__garbage__

#include <map>
#include <vector>
#include <boost/signals2.hpp>
namespace bs2 = boost::signals2;

#include "block.h"
#include "constants.h"
#include "tetromino.h"
#include "timer.h"


class Garbage
{
public:
    Garbage(size_t width = WORLD_N_BLOCKS_X, size_t height = WORLD_N_BLOCKS_Y);
    ~Garbage();
    
    void reset();
    
    void draw();
    void update();
    
    void pause();
    void unpause();
    
    bool isUpdating();
    bool isCascading();
    void gameOver();
    
    bool checkCollision(Tetromino piece);
    int getFilledTSpinCorners(Tetromino piece);
    int addTetromino(Tetromino piece);// returns lineClears

    void startCascade();
    int doCascade();// returns lineClears and updates 'isCascading'
    
    int top();
    
private:
    std::vector< std::vector<Block*> > blocks;
    std::list<int> pendingClearLines;
    Timer lineClearTimer;
    bool _isClearing;
    Timer cascadeTimer;
    bool _isCascading;
    enum BLOCK_STATE {
        TBD,
        EMPTY,
        STATIC
    };
    std::vector< std::vector<int> > cascadeStates;
    std::map< Block*, std::list<Block*> > connections;
    
    Timer gameOverTimer;
    int _gameOverRowIndex;
    bool _isGameOver;
    
    void markNextGameOverLine();
    
    int checkLineClears(std::list<int> rows);
    void markLinesForClearing(std::list<int> rows);
    void clearLines();
    
    int cascadeMarkStatic(int y, int x);// returns number of blocks marked
    void addConnections(std::list<Block*> newBlocks);
    void removeConnections(Block* block);
    
    void deleteAllBlocks();

};

#endif /* defined(__QuadBlocks__garbage__) */
