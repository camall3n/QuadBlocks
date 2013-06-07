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

World::World() :
    piece(TETROMINO::I),
    isPaused(false),
    baseGravity(1.0/60),
    gravity(baseGravity)
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
    
    piece.setPosition(glm::vec2(5,17));
    piece.setRotation(0);
}

void World::update()
{
    if (garbage.isClearing()) {
        garbage.update();
    }
    
    if (!isPaused) {
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
        
    }
}

void World::draw()
{
    light.makeActive();
    well.draw();
    garbage.draw();
    piece.draw();    
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
    
    if (!checkCollision(newPiece))
        piece = newPiece;

}

void World::moveDown()
{
    glm::vec2 pos = piece.position();
    
    pos.y--;
    Tetromino newPiece = piece;
    newPiece.setPosition(pos);
    
    if (!checkCollision(newPiece))
        piece = newPiece;
}

void World::moveRight()
{
    glm::vec2 pos = piece.position();

    pos.x++;
    Tetromino newPiece = piece;
    newPiece.setPosition(pos);
    
    if (!checkCollision(newPiece))
        piece = newPiece;
    else
        stopDragging();
}

void World::moveLeft()
{
    glm::vec2 pos = piece.position();
    
    pos.x--;
    Tetromino newPiece = piece;
    newPiece.setPosition(pos);
    
    if (!checkCollision(newPiece))
        piece = newPiece;
    else
        stopDragging();
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
    }
    else {
        Tetromino kickedPiece = tryWallKick(newPiece);
        if (kickedPiece != newPiece) {
            piece = kickedPiece;
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
    }
    else {
        Tetromino kickedPiece = tryWallKick(newPiece);
        if (kickedPiece != newPiece) {
            piece = kickedPiece;
        }
    }
}

void World::hardDrop()
{
    gravity = 20;
}
void World::softDrop()
{
    gravity = std::fmin(10*baseGravity, 20.0);
}
void World::normalDrop()
{
    gravity = baseGravity;
}

void World::hold()
{

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
    else if (gravity > 1.0 && gravity <= 20) {
        return floor(gravity);
    }
    else {
        std::cerr << "Error: Invalid gravity value. G = " << gravity << std::endl;
    }
    
    return 0;
}

void World::lock()
{
    garbage.addTetromino(piece);
    
    //  get new piece
    piece.setPosition(glm::vec2(5,17));
    piece.setRotation(0);
    if (checkCollision(piece)) {
        // top out!!
        std::cout << "TOP OUT!!" << std::endl;
    }
    
    //  update world traits
    
}




