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
#include <boost/signals2.hpp>
namespace bs2 = boost::signals2;

#include "block.h"
#include "camera.h"
#include "garbage.h"
#include "light.h"
#include "next_queue.h"
#include "scorekeeper.h"
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
    
    int getScore();
    int getLines();
    int getLevel();
    
    struct signalList {
        bs2::signal<void (int)> scoreChanged;
        bs2::signal<void (int)> newPoints;
        bs2::signal<void (int)> linesLeftChanged;
        bs2::signal<void (int)> levelChanged;
        bs2::signal<void (int, int)> timeChanged;
        bs2::signal<void ()> allClear;
        bs2::signal<void (int)> lineClear;
        bs2::signal<void (int, bool)> tSpin;
    } signal;

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
    ScoreKeeper scoreKeeper;
    

    // World traits
    bool holdingPiece;
    bool usedHoldPiece;
    bool isPaused;
    float baseGravity;
    float gravity;
    
    enum MOTION_TYPE {
        MOVE,
        FALL,
        SPIN,
        KICKSPIN,
        HOLD
    };
    MOTION_TYPE lastMotion;
    
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

    void updateUserPiece();
    bool checkCollision(Tetromino piece);
    bool checkTSpin(Tetromino piece);
    Tetromino wallKickCW(Tetromino piece);
    Tetromino wallKickCCW(Tetromino piece);
    Tetromino wallKick(Tetromino piece, int fromState, int rotation);
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
    
    // Scoring functions
    void newPoints(int points);
    void scoreChanged(int score);
    void linesChanged(int lines);
    void levelChanged(int level);
};

#endif /* defined(__Tetris__world__) */
