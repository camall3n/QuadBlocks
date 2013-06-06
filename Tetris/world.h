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
#include "well.h"

typedef const char direction_t;
namespace DIRECTION {
    direction_t NONE = 0x00;
    
    direction_t N = 0x01;
    direction_t S = 0x02;
    direction_t E = 0x03;
    direction_t W = 0x04;
    
    direction_t NW = N | W;
    direction_t NE = N | E;
    direction_t SW = S | W;
    direction_t SE = S | E;

    direction_t UP    = N;
    direction_t RIGHT = E;
    direction_t DOWN  = S;
    direction_t LEFT  = W;
}

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
    
    // World Functions
    void moveRight();
    void moveLeft();
    void rotateCW();
    void rotateCCW();
    void hardDrop();
    void softDrop();
    void hold();
    void pause();
    void unpause();
    
    // Dev Actions
    void moveUp();
    void moveDown();
    
private:
    Camera c;
    Tetromino piece;
    Light light;
    
    struct QueuedActions {
        bool moveRight = false;
        bool moveLeft = false;
        bool rotateCW = false;
        bool rotateCCW = false;
        bool hardDrop = false;
        bool softDrop = false;
        bool hold = false;
    } queuedAction;
    
    bool isPaused;
    
    float baseGravity;
    float gravity;
    
    void applyGravity();
    
    bool checkCollision(Tetromino piece);
    Tetromino tryWallKick(Tetromino piece);

    Well well;
    Garbage garbage;
    
};

#endif /* defined(__Tetris__world__) */
