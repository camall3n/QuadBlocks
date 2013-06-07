//
//  world.h
//  Tetris
//
//  Created by Cam Allen on 5/24/13.
//  Copyright (c) 2013 Cam Allen. All rights reserved.
//

#ifndef __Tetris__world__
#define __Tetris__world__

#include <list>
#include "block.h"
#include "camera.h"
#include "garbage.h"
#include "light.h"
#include "tetromino.h"
#include "timer.h"
#include "well.h"


class World
{
public:
    World();
    
    void update();
    void draw();

    // User Actions
    void queueMoveRight();
    void queueMoveLeft();
    void queueRotateCW();
    void queueRotateCCW();
    void queueHardDrop();
    void queueSoftDrop();
    void queueHold();
    void togglePause();
    
    // Dev Actions
    void moveUp();
    void moveDown();
    
private:
    // Objects
    Camera c;
    Tetromino piece;
    Light light;
    Well well;
    Garbage garbage;
    Timer lockTimer;//
    int nLockAttempts;//

    // World traits
    bool isPaused;
    float baseGravity;
    float gravity;
    
    // User actions
    struct QueuedActions {
        bool moveRight = false;
        bool moveLeft = false;
        bool rotateCW = false;
        bool rotateCCW = false;
        bool hardDrop = false;
        bool softDrop = false;
        bool hold = false;
    } queuedAction;
    
    // Piece movement
    void moveRight();
    void moveLeft();
    void rotateCW();
    void rotateCCW();
    void hold();

    bool checkCollision(Tetromino piece);
    Tetromino tryWallKick(Tetromino piece);
    void lock();
    
    // Gravity
    int getFallDistance();
    void hardDrop();
    void softDrop();
    void normalDrop();
    void applyGravity();

    // Menu functions
    void pause();
    void unpause();
    
};

#endif /* defined(__Tetris__world__) */
