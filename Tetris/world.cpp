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
#include "camera.h"
#include "constants.h"
#include "utility.h"

World::World() :
    piece(TETROMINO::I),
    gravity(1/60)
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
    
    piece.setPosition(glm::vec2(5,10));
    piece.setRotation(0);
}

void World::update()
{
    // Hold
    
    // Rotate
    
    // Horizontal shift
    
    // Gravity
    
    // Lock
}

void World::draw()
{
    light.makeActive();
    well.draw();
    garbage.draw();
    piece.draw();    
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
}

void World::moveLeft()
{
    glm::vec2 pos = piece.position();
    
    pos.x--;
    Tetromino newPiece = piece;
    newPiece.setPosition(pos);
    
    if (!checkCollision(newPiece))
        piece = newPiece;
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
//    std::cout << "Hard Drop" << std::endl;
}

void World::softDrop()
{
//    std::cout << "Soft Drop" << std::endl;
}

void World::hold()
{
//    std::cout << "Hold" << std::endl;
}

void World::pause()
{
//    std::cout << "Pause" << std::endl;
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


