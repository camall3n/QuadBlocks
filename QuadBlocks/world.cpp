//
//  world.cpp
//  QuadBlocks
//
//  Created by Cam Allen on 5/24/13.
//  Copyright (c) 2013 Cam Allen. All rights reserved.
//

#include "world.h"

#include <boost/foreach.hpp>
#include <iostream>
#include <cmath>
#include "camera.h"
#include "constants.h"
#include "kicktable.h"
#include "utility.h"

const double LOCK_DELAY = 0.5;
const double DRAG_DELAY = 0.2;
const double DRAG_REPEAT = 0.05;

// Gravity, in squares per frame
const double MIN_GRAVITY = 4/256;
const double MAX_GRAVITY = 4;
const double GRAVITY[] = {
      4/256.f,
      8/256.f,
     16/256.f,
     24/256.f,
     32/256.f,
     48/256.f,
     64/256.f,
     96/256.f,
    128/256.f,
    192/256.f,
    256/256.f,
    320/256.f,
    384/256.f,
    512/256.f,
    640/256.f
};
const double HARD_GRAVITY = 24;

const int TIMED_MODE_MINUTES = 2;

//#define DEVELOPER_MODE

World::World() :
  piece(TETROMINO::I),
  isPaused(false),
  baseGravity(MIN_GRAVITY),
  gravity(baseGravity),
  idxGravity(0),
  holdingPiece(false),
  usedHoldPiece(false),
  developerMode(false),
  gameOver(false),
  gameMode(TIMED)
{
    Block::useCamera(c);
    c.setPosition(glm::vec3(5,10,80));
    c.setTarget(glm::vec3(5,10,0));
    c.setOrientation(glm::vec3(0,1,0));
    c.setViewingAngle(30);
    c.makeActive();

    light.setPosition(glm::vec3(5,40,35));
    light.setLightColor(glm::vec3(1,1,1));
    light.setDarkColor(glm::vec3(0,0,0));
    light.setAttenuation(.15);
    Block::useLight(light);
    light.makeActive();
    
    piece = pieceQueue.getNext();
    piece.resetPosition();
    
    scoreKeeper.signal.newPoints.connect(
        boost::bind( &World::newPoints, this, _1)
    );
    scoreKeeper.signal.scoreChanged.connect(
        boost::bind( &World::scoreChanged, this, _1)
    );
    scoreKeeper.signal.linesChanged.connect(
        boost::bind( &World::linesChanged, this, _1)
    );
    scoreKeeper.signal.levelChanged.connect(
        boost::bind( &World::levelChanged, this, _1)
    );
}

void World::reset()
{
    isPaused = false;
    idxGravity = 0;
    scoreKeeper.reset();
    baseGravity = GRAVITY[idxGravity];
    gravity = baseGravity;
    holdingPiece = false;
    usedHoldPiece = false;
    gameOver = false;
    resetQueuedActions();
    
    pieceQueue.reset();
    garbage.reset();
    
    gameCountdown.setTime(TIMED_MODE_MINUTES, 0);
    gameTimer.stop();
    if (gameMode == TIMED) {
        signal.timeChanged(gameCountdown.getTimeString());
    } else if (gameMode == RACE) {
        signal.timeChanged(gameTimer.getTimeString());
    } else {
        signal.timeChanged("");
    }
    signal.levelChanged(getLevel());
    signal.linesLeftChanged(getLines());
    signal.scoreChanged(getScore());
    gameCountdown.start();
    gameTimer.start();
    
    piece = pieceQueue.getNext();
    piece.resetPosition();
}

void World::setMode(GAME_MODE mode)
{
    gameMode = mode;
}

void World::resetQueuedActions()
{
    queuedAction.moveRight = false;
    queuedAction.dragRight = false;
    queuedAction.moveLeft = false;
    queuedAction.dragLeft = false;
    queuedAction.rotateCW = false;
    queuedAction.rotateCCW = false;
    queuedAction.hardDrop = false;
    queuedAction.softDrop = false;
    queuedAction.hold = false;
}

void World::update()
{
    if (gameOver) {
        soundboard.PauseMusic();
        if (garbage.isUpdating()) {
            garbage.update();
            
            // If that was the last garbage update, broadcast gameOver signal
            if (!garbage.isUpdating()) {
                soundboard.GameOver();
                signal.gameOver();
            }
        }
    }
    else if (!isPaused) {
        if (gameMode == TIMED) {
            if (gameCountdown.didChange()) {
                signal.timeChanged(gameCountdown.getTimeString());
            }
            if (!gameOver && gameCountdown.isStarted() && gameCountdown.isDone()) {
                // Game Over
                gameOver = true;
                garbage.gameOver();
                return;
            }
        }
        
        soundboard.PlayMusic();
        pieceQueue.update();
        garbage.update();
    
        if (garbage.isCascading()) {
            int linesCleared = garbage.doCascade();
            if (linesCleared > 0) {
                soundboard.LineClear();
                scoreKeeper.scoreCascade(linesCleared);
            }
        }
        else {
            // end chain
            scoreKeeper.resetCascade();
        }
        updateUserPiece();
    }
    else {
        pause();
    }
}

void World::updateUserPiece() {
    // Hold
    if (queuedAction.hold) {
        hold();
        queuedAction.hold = false;
    }
    
    // Rotation
    bool didRotate = false;
    if (queuedAction.rotateCW) {
        didRotate = rotateCW();
        queuedAction.rotateCW = false;
    }
    else if (queuedAction.rotateCCW) {
        didRotate = rotateCCW();
        queuedAction.rotateCCW = false;
    }
    
    // Horizontal Movement
    bool didMove = false;
    if (queuedAction.moveLeft) {
        didMove = moveLeft();
        stopDragging();
    }
    else if (queuedAction.moveRight) {
        didMove = moveRight();
        stopDragging();
    }
    else if (queuedAction.dragLeft) {
        didMove = dragLeft();
    }
    else if (queuedAction.dragRight) {
        didMove = dragRight();
    }
    
    // Gravity
    applyGravity(didMove, didRotate);
    
    updateGhostPiece();
}

void World::draw()
{
    light.makeActive();
    well.draw();
    garbage.draw();
    pieceQueue.draw();
    
    piece.draw();
    ghostPiece.draw();
    
    if (holdingPiece) {
        holdPiece.draw();
    }
}

void World::queueMoveRight() {
    if (!isPaused) {
        queuedAction.moveRight = true;
        queuedAction.moveLeft = false;
        queuedAction.dragLeft = false;
    }
}
void World::queueMoveLeft() {
    if (!isPaused) {
        queuedAction.moveLeft = true;
        queuedAction.moveRight = false;
        queuedAction.dragRight = false;
    }
}
void World::queueDragRight() {
    if (!isPaused) {
        queuedAction.dragRight = true;
        queuedAction.dragLeft = false;
    }
}
void World::queueDragLeft() {
    if (!isPaused) {
        queuedAction.dragLeft = true;
        queuedAction.dragRight = false;
    }
}
void World::queueRotateCW() {
    if (!isPaused) {
        queuedAction.rotateCW = true;
        queuedAction.rotateCCW = false;
    }
}
void World::queueRotateCCW() {
    if (!isPaused) {
        queuedAction.rotateCCW = true;
        queuedAction.rotateCW = false;
    }
}
void World::queueHardDrop() {
    if (!isPaused) {
        queuedAction.hardDrop = true;
        queuedAction.softDrop = false;
    }
}
void World::queueSoftDrop() {
    if (!isPaused) {
        queuedAction.softDrop = true;
        queuedAction.hardDrop = false;
    }
}
void World::queueHold() {
    if (!isPaused) {
        queuedAction.hold = true;
    }
}
void World::togglePause() {
    if (isPaused) {
        unpause();
    }
    else {
        pause();
        soundboard.PauseMusic();
    }
}
void World::toggleDevMode()
{
    developerMode = !developerMode;
}

void World::moveUp()
{
    glm::vec2 pos = piece.position();
    
    pos.y++;
    Tetromino newPiece = piece;
    newPiece.setPosition(pos);
    
    if (!checkCollision(newPiece)) {
        piece = newPiece;
        lastMotion = MOVE;
        soundboard.Move();
    }

}

void World::moveDown()
{
    glm::vec2 pos = piece.position();
    
    pos.y--;
    Tetromino newPiece = piece;
    newPiece.setPosition(pos);
    
    if (!checkCollision(newPiece)) {
        piece = newPiece;
        lastMotion = MOVE;
        soundboard.Move();
    }
}

bool World::moveRight()
{
    glm::vec2 pos = piece.position();

    pos.x++;
    Tetromino newPiece = piece;
    newPiece.setPosition(pos);
    
    if (!checkCollision(newPiece)) {
        piece = newPiece;
        lastMotion = MOVE;
        soundboard.Move();
        return true;
    }
    else {
        stopDragging();
        return false;
    }
}

bool World::moveLeft()
{
    glm::vec2 pos = piece.position();
    
    pos.x--;
    Tetromino newPiece = piece;
    newPiece.setPosition(pos);
    
    if (!checkCollision(newPiece)) {
        piece = newPiece;
        lastMotion = MOVE;
        soundboard.Move();
        return true;
    }
    else {
        stopDragging();
        return false;
    }
}

bool World::dragRight()
{
    dragTimerLeft.stop();
    queuedAction.dragLeft = false;
    
    bool didMove = false;
    
    if (dragTimerRight.isStarted())
    {
        static double repeat = 0.0;
        if (dragTimerRight.getTime() > DRAG_DELAY) {
            if (repeat==0.0 || repeat > DRAG_REPEAT) {
                didMove = moveRight();
                repeat = 0.0;
            }
            repeat += 1.0/FRAMES_PER_SECOND;
        }
        else {
            repeat = 0.0;
        }
    }
    else {
        dragTimerRight.start();
    }
    return didMove;
}

bool World::dragLeft()
{
    dragTimerRight.stop();
    queuedAction.dragRight = false;
    
    bool didMove = false;
    
    if (dragTimerLeft.isStarted())
    {
        static double repeat = 0.0;
        if (dragTimerLeft.getTime() > DRAG_DELAY) {
            if (repeat==0.0 || repeat > DRAG_REPEAT) {
                didMove = moveLeft();
                repeat = 0.0;
            }
            repeat += 1.0/FRAMES_PER_SECOND;
        }
        else {
            repeat = 0.0;
        }
    }
    else {
        dragTimerLeft.start();
    }
    return didMove;
}

void World::stopDragging()
{
    queuedAction.moveRight = false;
    queuedAction.dragRight = false;
    queuedAction.moveLeft = false;
    queuedAction.dragLeft = false;
    dragTimerLeft.stop();
    dragTimerRight.stop();
}

bool World::rotateCW()
{
    float angle = piece.rotation();
    
    angle+=.25;
    Tetromino newPiece = piece;
    newPiece.setRotation(angle);
    
    if (!checkCollision(newPiece)) {
        piece = newPiece;
        lastMotion = SPIN;
        soundboard.Rotate();
        return true;
    }
    else {
        Tetromino kickedPiece = wallKickCW(newPiece);
        if (kickedPiece != newPiece) {
            piece = kickedPiece;
            lastMotion = KICKSPIN;
            soundboard.Rotate();
            return true;
        }
    }
    return false;
}

bool World::rotateCCW()
{
    float angle = piece.rotation();
    
    angle-=.25;
    Tetromino newPiece = piece;
    newPiece.setRotation(angle);
    
    if (!checkCollision(newPiece)) {
        piece = newPiece;
        lastMotion = SPIN;
        soundboard.Rotate();
        return true;
    }
    else {
        Tetromino kickedPiece = wallKickCCW(newPiece);
        if (kickedPiece != newPiece) {
            piece = kickedPiece;
            lastMotion = KICKSPIN;
            soundboard.Rotate();
            return true;
        }
    }
    return false;
}

void World::hardDrop()
{
    gravity = HARD_GRAVITY;
}
void World::softDrop()
{
    gravity = 10*baseGravity;// Try 10 * base
    gravity = std::fmax(gravity, 0.5);// make sure it's at least 0.5
    gravity = std::fmin(gravity, MAX_GRAVITY);// make sure it's less than max
}
void World::normalDrop()
{
    gravity = baseGravity;
}

void World::hold()
{
    if (developerMode) {
        usedHoldPiece = false;
        holdingPiece = false;
    }
    if (!usedHoldPiece) {
        usedHoldPiece = true;
        if (holdingPiece) {
            Tetromino temp = holdPiece;
            temp.resetPosition();
            
            piece.holdPosition();
            holdPiece = piece;
            
            piece = temp;
            lastMotion = HOLD;
        }
        else {
            holdPiece = piece;
            
            holdPiece.holdPosition();
            
            piece = pieceQueue.getNext();
            piece.resetPosition();
            holdingPiece = true;
            lastMotion = HOLD;
        }
        // Cancel any in-progress piece-locking
        lockTimer.stop();
        infinityTimer.stop();
    }
    else {
        // Add sound?
    }
}

void World::pause()
{
    gameCountdown.pause();
    gameTimer.pause();
    soundboard.PauseMusic();
    isPaused = true;
}

void World::unpause()
{
    // TODO: Display "get ready" or something?
    if (gameTimer.isStarted()) {
        gameTimer.unpause();
    }
    else {
        gameTimer.start();
    }
    if (gameCountdown.isStarted()) {
        gameCountdown.unpause();
    }
    else {
        gameCountdown.start();
    }
    
    soundboard.PlayMusic();
    isPaused = false;
}


void World::applyGravity(bool didMove, bool didRotate)
{
    // Gravity Modifiers
    if (queuedAction.hardDrop) {
        hardDrop();
    }
    else if (queuedAction.softDrop) {
        softDrop();
    }
    
    // Account for gravity speed
    int maximumFall = getFallDistance();
    int actualFall = 0;
    
    // Compute new piece position
    bool atBottom = false;
    for (int i=1; i<=maximumFall; i++) {
        Tetromino newPiece(piece);
        glm::vec2 pos = piece.position();
        pos += glm::vec2(0, -1);
        newPiece.setPosition(pos);
        if (checkCollision(newPiece)) {
            atBottom = true;
            break;
        }
        else {
            piece = newPiece;
            lastMotion = FALL;
            actualFall++;
        }
    }
    
    if (actualFall>0) {
        if (queuedAction.hardDrop) {
            scoreKeeper.hardDrop(actualFall);
        }
        else if (queuedAction.softDrop) {
            scoreKeeper.softDrop(actualFall);
//            soundboard.Move();
        }
    }
    
    if ( (lockTimer.isStarted() && (didMove || didRotate)) &&
         ((infinityTimer.getTime() < maxInfinity) || (maxInfinity < 0)) )
    {
        // Saved by "Infinity"!
        lockTimer.stop();
        lockTimer.start();
    }
    
    // If the piece was supposed to fall
    if (maximumFall>0) {
        // But it's already at the bottom!
        if (atBottom) {
            // Do lock-related things
            if (queuedAction.hardDrop) {
                soundboard.Drop();
                lock();
                normalDrop();
                lockTimer.stop();
                infinityTimer.stop();
            }
            else if (lockTimer.isStarted()) {
                if (lockTimer.getTime() > LOCK_DELAY) {
                    lock();
                    normalDrop();
                    lockTimer.stop();
                    infinityTimer.stop();
                }
            }
            else {
                lockTimer.start();
                if (!infinityTimer.isStarted()) {
                    infinityTimer.start();
                }
            }
        }
        // It's not at the bottom...
        else {
            lockTimer.stop();
        }
    }
    
    // Turn off gravity modifiers
    if (queuedAction.hardDrop || queuedAction.softDrop) {
        normalDrop();
        queuedAction.hardDrop = false;
        queuedAction.softDrop = false;
    }
}

bool World::checkCollision(Tetromino piece)
{
    return ( well.checkCollision(piece) || garbage.checkCollision(piece) );
}

bool World::checkTSpin(Tetromino piece)
{
    if (piece.type() != TETROMINO::T) {
        return false;
    }
    
    if (lastMotion != SPIN && lastMotion != KICKSPIN) {
        return false;
    }
    
    if (garbage.getFilledTSpinCorners(piece) < 3) {
        return false;
    }
    
    return true;
}

Tetromino World::wallKickCW(Tetromino piece)
{
    float rotation = piece.rotation()*4;
    int toState = floor(rotation);
    int fromState = (toState+3)%4;
    
    return wallKick(piece, fromState, 1); // CW
}

Tetromino World::wallKickCCW(Tetromino piece)
{
    float rotation = piece.rotation()*4;
    int toState = floor(rotation);
    int fromState = (toState+1)%4;

    return wallKick(piece, fromState, 0); // CCW
}

Tetromino World::wallKick(Tetromino piece, int fromState, int rotation)
{
    static KickTable kick;
    
    if (piece.type() == TETROMINO::O)
        return piece;
    
    Tetromino kickedPiece(piece);
    glm::vec2 basePos = piece.position();
    
    for (int i=1; i<5; i++) {
        glm::vec2 offset;
        if (piece.type() == TETROMINO::I) {
            offset = kick.getIOffset(fromState, rotation, i);
        }
        else {
            offset = kick.getOffset(fromState, rotation, i);
        }
        kickedPiece.setPosition(basePos + offset);
        if (!checkCollision(kickedPiece)) {
            return kickedPiece;
        }
    }
    
    return piece;
}

int World::getFallDistance()
{
    if (gravity > 0.0 && gravity < 1.0) {
        static float lowGravityCounter = 0.0;
        
        if (lowGravityCounter >= 1.0) {
            lowGravityCounter = 0.0;
            return 1;
        }
        else {
            lowGravityCounter += gravity;
            return 0;
        }
    }
    else if (gravity == 1.0) {
        return 1;
    }
    else if (gravity > 1.0 && gravity <= HARD_GRAVITY) {
        return round(gravity);
    }
    else {
        std::cerr << "Error: Invalid gravity value. G = " << gravity << std::endl;
    }
    
    return 0;
}

void World::lock()
{
    bool hadTSpin = checkTSpin(piece);
    bool hadKick = (lastMotion == KICKSPIN);
    bool sendingMessage = false;// flag to give "bravo" precedence
    
    int linesCleared = garbage.addTetromino(piece);
    if (linesCleared < 0) {
        // Game Over
        gameOver = true;
        garbage.gameOver();
    }
    else {
        if (garbage.top() <= linesCleared) {
            scoreKeeper.queueBravo();
            signal.allClear();
            sendingMessage = true;
        }
        if (hadTSpin) {
            scoreKeeper.tSpin(linesCleared, hadKick);
            if (!sendingMessage) {
                signal.tSpin(linesCleared, hadKick);
            }
        }
        else {
            scoreKeeper.linesCleared(linesCleared);
            if (!sendingMessage && linesCleared > 0) {
                signal.lineClear(linesCleared);
            }
        }
        
        if (linesCleared > 0) {
            soundboard.LineClear();
            if (sendingMessage) {
                soundboard.AllClear();
            }
            else
            switch (linesCleared) {
                case 1:
                    soundboard.Single();
                    break;
                case 2:
                    soundboard.Double();
                    break;
                case 3:
                    soundboard.Triple();
                    break;
                case 4:
                    soundboard.Quad();
                    break;
                    
                default:
                    std::cerr << "Error: linesCleared = " << linesCleared << std::endl;
                    break;
            }
        }
    
        Tetromino nextPiece = pieceQueue.getNext();
        piece = nextPiece;
        usedHoldPiece = false;
        
        piece.resetPosition();
        if (checkCollision(piece)) {
            // Game Over
            gameOver = true;
            garbage.gameOver();
        }
    }
}

void World::updateGhostPiece()
{
    ghostPiece = piece;
    
    ghostPiece.setState(TETROMINO::GHOST);
    
    for (int i=1; i<=HARD_GRAVITY; i++) {
        Tetromino newPiece(ghostPiece);
        glm::vec2 pos = ghostPiece.position();
        pos += glm::vec2(0, -1);
        newPiece.setPosition(pos);
        if (checkCollision(newPiece)) {
            break;
        }
        else {
            ghostPiece = newPiece;
        }
    }
}

int World::getScore() {
    return scoreKeeper.getScore();
}
int World::getLines() {
    return scoreKeeper.getLines();
}
int World::getLevel() {
    return scoreKeeper.getLevel();
}

void World::newPoints(int points) {
    signal.newPoints(points);
}
void World::scoreChanged(int score) {
    signal.scoreChanged(score);
}
void World::linesChanged(int lines) {
    signal.linesLeftChanged(lines);
}
void World::levelChanged(int level) {
    signal.levelChanged(level);
    idxGravity = level-1;
    baseGravity = GRAVITY[idxGravity];
    gravity = baseGravity;
    soundboard.LevelUp();
    gameCountdown.reset();
    signal.timeChanged(gameCountdown.getTimeString());
    gameCountdown.start();
}


