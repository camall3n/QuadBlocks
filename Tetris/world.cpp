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
    piece(TETROMINO::T)
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

void World::moveRight()
{
    glm::vec2 pos = piece.position();

    pos.x++;
    Tetromino newPiece = piece;
    newPiece.setPosition(pos);
    
    piece = newPiece;
}

void World::moveLeft()
{
    glm::vec2 pos = piece.position();
    
    pos.x--;
    Tetromino newPiece = piece;
    newPiece.setPosition(pos);
    
    piece = newPiece;
}
