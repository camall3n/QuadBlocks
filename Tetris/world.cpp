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
#include "utility.h"

World::World() :
    piece(TETROMINO::S)
{
    for (int i=0; i<10; i++) {
        for (int j=0; j<22; j++) {
//            Block b = Block(BLOCK::COLOR::NUMBER[(i*11+j) % 7]);
//            Block b = Block(BLOCK::COLOR::NUMBER[randint(0, BLOCK::COLOR::MAX_NUMBER)]);
            Block b = Block(BLOCK::COLOR::BLACK);
            b.setPosition(glm::vec3(i+0.5, j+0.5,-0.2));
            blocks.push_back(b);
        }
    }
        
    Block::useCamera(c);
    Block::useLight(topLight);

    c.setPosition(glm::vec3(5,10,80));
    c.setTarget(glm::vec3(5,10,0));
    c.setOrientation(glm::vec3(0,1,0));
    c.setViewingAngle(30);
    c.makeActive();

    topLight.setPosition(glm::vec3(5,40,35));
    topLight.setIntensity(glm::vec4(.5,.5,.5,1));
//    topLight.setAmbientIntensity(glm::vec4(.1,.1,.1,1));
    topLight.setAmbientIntensity(glm::vec4(0,0,0,1));
    topLight.setAttenuation(.15);

    frontLight.setPosition(glm::vec3(5,10,80));
    frontLight.setIntensity(glm::vec4(.5,.5,.5,1));
    //    frontLight.setAmbientIntensity(glm::vec4(.1,.1,.1,1));
    frontLight.setAmbientIntensity(glm::vec4(0,0,0,1));
    frontLight.setAttenuation(.15);
    
    frontLight.makeActive();
        
    piece.setPosition(glm::vec2(0,0));
    piece.setRotation(0);
    
    std::cout << piece.collisionSquare() << std::endl;
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
    frontLight.makeActive();
    BOOST_FOREACH(Block block, blocks) {
        block.draw();
    }
    topLight.makeActive();
    piece.draw();
}

