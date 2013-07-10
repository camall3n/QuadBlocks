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
#include "utility.h"

const double LOCK_DELAY = 1.0;
const double DRAG_DELAY = 0.25;
const double DRAG_REPEAT = 0.1;

const double MIN_GRAVITY = 1.0/FRAMES_PER_SECOND;
const double MAX_GRAVITY = 20;
const glm::vec2 startingPos(4,18);

World::World() :
    piece(TETROMINO::I),
    isPaused(false),
    baseGravity(MIN_GRAVITY),
    gravity(baseGravity),
    holdingPiece(false),
    usedHoldPiece(false),
    _isDirty(true)
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
    if (piece.type() == TETROMINO::TYPE::I) {
        piece.setPosition(startingPos + glm::vec2(-1,0));
    }
    else {
        piece.setPosition(startingPos);
    }
    piece.setRotation(0);
    
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
    pieceQueue.update();
    
    if (garbage.isClearing()) {
        garbage.update();
    }
    
    if (!isPaused) {
        updateUserPiece();
    }
    else {
        _isDirty = true;
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
    if (true || _isDirty) {
        light.makeActive();
        well.draw();
        garbage.draw();
        pieceQueue.draw();
        
        ghostPiece.draw();
        piece.draw();
        
        if (holdingPiece) {
            holdPiece.draw();
        }
        _isDirty = false;
    }
}

bool World::isDirty() {
    return _isDirty;
}


void World::queueMoveRight() {
    queuedAction.moveRight = true;
    queuedAction.moveLeft = false;
    queuedAction.dragLeft = false;
}
void World::queueMoveLeft() {
    queuedAction.moveLeft = true;
    queuedAction.moveRight = false;
    queuedAction.dragRight = false;
}
void World::queueDragRight() {
    queuedAction.dragRight = true;
    queuedAction.dragLeft = false;
}
void World::queueDragLeft() {
    queuedAction.dragLeft = true;
    queuedAction.dragRight = false;
}
void World::queueRotateCW() {
    queuedAction.rotateCW = true;
    queuedAction.rotateCCW = false;
}
void World::queueRotateCCW() {
    queuedAction.rotateCCW = true;
    queuedAction.rotateCW = false;
}
void World::queueHardDrop() {
    queuedAction.hardDrop = true;
    queuedAction.softDrop = false;
}
void World::queueSoftDrop() {
    queuedAction.softDrop = true;
    queuedAction.hardDrop = false;
}
void World::queueHold() {
    queuedAction.hold = true;
}
void World::togglePause() {
    if (isPaused) {
        unpause();
    }
    else {
        pause();
    }
}

void World::moveUp()
{
    glm::vec2 pos = piece.position();
    
    pos.y++;
    Tetromino newPiece = piece;
    newPiece.setPosition(pos);
    
    if (!checkCollision(newPiece)) {
        piece = newPiece;
        _isDirty = true;
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
        _isDirty = true;
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
        _isDirty = true;
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
        _isDirty = true;
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
        _isDirty = true;
    }
    else {
        Tetromino kickedPiece = tryWallKick(newPiece);
        if (kickedPiece != newPiece) {
            piece = kickedPiece;
            _isDirty = true;
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
        _isDirty = true;
    }
    else {
        Tetromino kickedPiece = tryWallKick(newPiece);
        if (kickedPiece != newPiece) {
            piece = kickedPiece;
            _isDirty = true;
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
    if (!usedHoldPiece) {
        usedHoldPiece = true;
        if (holdingPiece) {
            Tetromino temp = holdPiece;
            temp.setPosition(startingPos);
            temp.setRotation(piece.rotation());
            
            piece.setPosition(holdPiece.position());
            piece.setRotation(holdPiece.rotation());
            holdPiece = piece;
            
            piece = temp;
            _isDirty = true;
        }
        else {
            holdPiece = piece;
            holdPiece.setPosition(glm::vec2(-5,WORLD_N_BLOCKS_Y-4));
            piece = pieceQueue.getNext();
            piece.setPosition(startingPos);
            holdingPiece = true;
            _isDirty = true;
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
            actualFall++;
            _isDirty = true;
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

Tetromino World::tryWallKick(Tetromino piece)
{
    Tetromino tryRight = piece;
    tryRight.setPosition(piece.position()+glm::vec2(1,0));
    
    Tetromino tryLeft = piece;
    tryLeft.setPosition(piece.position()+glm::vec2(-1,0));
    
    Tetromino tryUp = piece;
    tryUp.setPosition(piece.position()+glm::vec2(0,1));
    
//    Tetromino tryDown = piece;
//    tryDown.setPosition(piece.position()+glm::vec2(0,-1));
    
    if (!checkCollision(tryRight)) {
        piece = tryRight;
    }
    else if (!checkCollision(tryLeft)) {
        piece = tryLeft;
    }
    else if (!checkCollision(tryUp)) {
        piece = tryUp;
    }
//    else if (!checkCollision(tryDown)) {
//        piece = tryDown;
//    }
    else {
        Tetromino tryDoubleRight = tryRight;
        tryDoubleRight.setPosition(tryRight.position()+glm::vec2(1,0));
        
        Tetromino tryDoubleLeft = tryLeft;
        tryDoubleLeft.setPosition(tryLeft.position()+glm::vec2(-1,0));
        
        Tetromino tryDoubleUp = tryUp;
        tryDoubleUp.setPosition(tryUp.position()+glm::vec2(0,1));
        
        if (!checkCollision(tryDoubleRight)) {
            piece = tryDoubleRight;
        }
        else if (!checkCollision(tryDoubleLeft)) {
            piece = tryDoubleLeft;
        }
        else if (!checkCollision(tryDoubleUp)) {
            piece = tryDoubleUp;
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
    int linesCleared = garbage.addTetromino(piece);
    scoreKeeper.linesCleared(linesCleared);
    
    Tetromino nextPiece = pieceQueue.getNext();
    piece = nextPiece;
    usedHoldPiece = false;
    
    if (piece.type() == TETROMINO::TYPE::I) {
        piece.setPosition(startingPos + glm::vec2(-1,0));
    }
    else {
        piece.setPosition(startingPos);
    }
    piece.setRotation(0);
    if (checkCollision(piece)) {
        // top out!!
        std::cout << "TOP OUT!!" << std::endl;
    }
    
    //  update world traits?
    
    _isDirty = true;
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
            _isDirty = true;
        }
    }
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


