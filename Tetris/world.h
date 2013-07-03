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
#include "next_queue.h"
#include "tetromino.h"
#include "timer.h"
#include "well.h"


class World
{
public:
    World();
    
    void update();
    void draw();
    bool isDirty();

    // User Actions
    void queueMoveRight();
    void queueMoveLeft();
    
    void queueDragRight();
    void queueDragLeft();
    void stopDragging();
    
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
    Tetromino holdPiece;
    Tetromino ghostPiece;
    Light light;
    Well well;
    Garbage garbage;
    NextQueue pieceQueue;
    Timer lockTimer;
    Timer dragTimerRight;
    Timer dragTimerLeft;

    // World traits
    bool holdingPiece;
    bool usedHoldPiece;
    bool isPaused;
    float baseGravity;
    float gravity;
    
    // Other variables
    bool _isDirty;
    
    // User actions
    struct QueuedActions {
        bool moveRight = false;
        bool dragRight = false;
        bool moveLeft = false;
        bool dragLeft = false;
        bool rotateCW = false;
        bool rotateCCW = false;
        bool hardDrop = false;
        bool softDrop = false;
        bool hold = false;
    } queuedAction;
    
    // Piece movement
    void moveRight();
    void dragRight();
    void moveLeft();
    void dragLeft();
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
    void updateGhostPiece();

    // Menu functions
    void pause();
    void unpause();
    
};

#endif /* defined(__Tetris__world__) */
