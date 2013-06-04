//
//  well.cpp
//  Tetris
//
//  Created by Cam Allen on 6/1/13.
//  Copyright (c) 2013 Cam Allen. All rights reserved.
//

#include "well.h"
#include <boost/foreach.hpp>
#include <iostream>
#include "utility.h"

Well::Well(size_t width, size_t height)
{
    glm::vec4 color = BLOCK::COLOR::BLACK;
    
    for (int i = -1; i < (int)height+1; i++) {
        Block left = Block(color);
        Block right = left;
        left.setPosition(glm::vec2(-0.5, i+0.5));
        right.setPosition(glm::vec2(-0.5+width+1, i+0.5));
        border.push_back(left);
        border.push_back(right);
    }
    
    for (int i=0; i<(int)width; i++) {
        Block block = Block(color);
        block.setPosition(glm::vec2(i+0.5, -0.5));
        border.push_back(block);
    }
    
    for (int i=0; i<(int)width; i++) {
        for (int j=0; j<(int)height; j++) {
            Block b = Block(BLOCK::COLOR::BLACK);
            b.setPosition(glm::vec3(i+0.5, j+0.5, 0));
            backWall.push_back(b);
        }
    }
    
    frontLight.setPosition(glm::vec3(5,10,80));
    frontLight.setLightColor(glm::vec3(0.5,0.5,0.5));
    frontLight.setDarkColor(glm::vec3(0,0,0));
    frontLight.setAttenuation(.15);
}

void Well::draw()
{
    // Store the active light
    Light* activeLight = Light::getActiveLight();
    
    // Front-light the backWall blocks
    frontLight.makeActive();
    BOOST_FOREACH(Block block, backWall) {
        block.draw();
    }
    
    // Return to the previous active light and draw the rest of the blocks
    activeLight->makeActive();
    BOOST_FOREACH(Block block, border) {
        block.draw();
    }
    
}

void Well::setFrontLight(Light light)
{
    frontLight = light;
}

void Well::setTopLight(Light light)
{
    topLight = light;
}

bool Well::checkCollision(Tetromino piece)
{
    float leftWallX = 0;
    float rightWallX = WORLD_N_BLOCKS_X;
    float floorY = 0;
    
    glm::vec2 basePosition = piece.position();
    glm::mat4 blocks = piece.collisionSquare();
    
    for (int i=0; i<piece.collisionSquareSize(); i++) {
        for (int j=0; j<piece.collisionSquareSize(); j++) {
            if (blocks[i][j]) {
                glm::vec2 position = basePosition + glm::vec2(j+0.5, i+0.5);
                if ((position.x < leftWallX) ||
                    (position.x > rightWallX) ||
                    (position.y < floorY))
                {
                    return true;
                }
            }
        }
    }
    
    return false;
}


