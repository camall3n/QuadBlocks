//
//  world.cpp
//  Tetris
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

const double LOCK_DELAY = 1.0;
const double DRAG_DELAY = 0.25;
const double DRAG_REPEAT = 0.1;

const double MIN_GRAVITY = 1.0/FRAMES_PER_SECOND;
const double MAX_GRAVITY = 24;

//#define DEVELOPER_MODE

World::World() :
    piece(TETROMINO::I),
    isPaused(false),
    baseGravity(MIN_GRAVITY),
    gravity(baseGravity),
    holdingPiece(false),
    usedHoldPiece(false),
    developerMode(false),
    gameOver(false)
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

void World::update()
{
    if (gameOver) {
        
    }
    else if (!isPaused) {
        pieceQueue.update();
        
        if (garbage.isClearing()) {
            garbage.update();
        }
    
        updateUserPiece();
    }
}

void World::updateUserPiece() {
    // Hold
    if (queuedAction.hold) {
        hold();
        queuedAction.hold = false;
    }
    
    // Rotation
    if (queuedAction.rotateCW) {
        rotateCW();
        queuedAction.rotateCW = false;
    }
    else if (queuedAction.rotateCCW) {
        rotateCCW();
        queuedAction.rotateCCW = false;
    }
    
    // Horizontal Movement
    if (queuedAction.moveLeft) {
        moveLeft();
        stopDragging();
    }
    else if (queuedAction.moveRight) {
        moveRight();
        stopDragging();
    }
    else if (queuedAction.dragLeft) {
        dragLeft();
    }
    else if (queuedAction.dragRight) {
        dragRight();
    }
    
    // Gravity
    applyGravity();
    
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
    }
}

void World::moveRight()
{
    glm::vec2 pos = piece.position();

    pos.x++;
    Tetromino newPiece = piece;
    newPiece.setPosition(pos);
    
    if (!checkCollision(newPiece)) {
        piece = newPiece;
        lastMotion = MOVE;
    }
    else {
        stopDragging();
    }
}

void World::moveLeft()
{
    glm::vec2 pos = piece.position();
    
    pos.x--;
    Tetromino newPiece = piece;
    newPiece.setPosition(pos);
    
    if (!checkCollision(newPiece)) {
        piece = newPiece;
        lastMotion = MOVE;
    }
    else {
        stopDragging();
    }
}

void World::dragRight()
{
    dragTimerLeft.stop();
    queuedAction.dragLeft = false;
    
    if (dragTimerRight.isStarted())
    {
        static double repeat = 0.0;
        if (dragTimerRight.getTime() > DRAG_DELAY) {
            if (repeat==0.0 || repeat > DRAG_REPEAT) {
                moveRight();
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
}

void World::dragLeft()
{
    dragTimerRight.stop();
    queuedAction.dragRight = false;
    
    if (dragTimerLeft.isStarted())
    {
        static double repeat = 0.0;
        if (dragTimerLeft.getTime() > DRAG_DELAY) {
            if (repeat==0.0 || repeat > DRAG_REPEAT) {
                moveLeft();
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

void World::rotateCW()
{
    float angle = piece.rotation();
    
    angle+=.25;
    Tetromino newPiece = piece;
    newPiece.setRotation(angle);
    
    if (!checkCollision(newPiece)) {
        piece = newPiece;
        lastMotion = SPIN;
    }
    else {
        Tetromino kickedPiece = wallKickCW(newPiece);
        if (kickedPiece != newPiece) {
            piece = kickedPiece;
            lastMotion = KICKSPIN;
        }
    }
}

void World::rotateCCW()
{
    float angle = piece.rotation();
    
    angle-=.25;
    Tetromino newPiece = piece;
    newPiece.setRotation(angle);
    
    if (!checkCollision(newPiece)) {
        piece = newPiece;
        lastMotion = SPIN;
    }
    else {
        Tetromino kickedPiece = wallKickCCW(newPiece);
        if (kickedPiece != newPiece) {
            piece = kickedPiece;
            lastMotion = KICKSPIN;
        }
    }
}

void World::hardDrop()
{
    gravity = MAX_GRAVITY;
}
void World::softDrop()
{
    gravity = std::fmin(10*baseGravity, MAX_GRAVITY);
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
    }
    else {
        // Add sound?
    }
}

void World::pause()
{
    isPaused = true;
    
    // Display menu?
}

void World::unpause()
{
    // Display "get ready" or something?
    
    isPaused = false;
}


void World::applyGravity()
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
        }
    }
    
    if (maximumFall>0) {
        // If the piece was supposed to fall
        
        if (atBottom) {
            // Do lock-related things if necessary

            if (queuedAction.hardDrop) {
                lock();
                normalDrop();
                lockTimer.stop();
            }
            
            if (lockTimer.isStarted()) {
                if (lockTimer.getTime() > LOCK_DELAY) {
                    lock();
                    normalDrop();
                    lockTimer.stop();
                }
            }
            else {
                lockTimer.start();
            }
        }
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
    else if (gravity > 1.0 && gravity <= MAX_GRAVITY) {
        return floor(gravity);
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
        // top out!!
        std::cout << "TOP OUT!!" << std::endl;
        gameOver = true;
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
    
        Tetromino nextPiece = pieceQueue.getNext();
        piece = nextPiece;
        usedHoldPiece = false;
        
        piece.resetPosition();
        if (checkCollision(piece)) {
            // top out!!
            std::cout << "TOP OUT!!" << std::endl;
        }
    }
}

void World::updateGhostPiece()
{
    ghostPiece = piece;
    
    ghostPiece.setState(TETROMINO::GHOST);
    
    for (int i=1; i<=MAX_GRAVITY; i++) {
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
    baseGravity *= 1.5;
    gravity = baseGravity;
}


