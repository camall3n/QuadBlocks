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
    void moveRight();
    void moveLeft();
    void rotateCW();
    void rotateCCW();
    void hold();
    void pause();
    
private:
    Camera c;
    Tetromino piece;
    Light light;
    
    direction_t checkCollision(Tetromino piece, Well well);
    direction_t checkCollision(Tetromino piece, Garbage garbage);

    Well well;
    Garbage garbage;
    
};

#endif /* defined(__Tetris__world__) */
