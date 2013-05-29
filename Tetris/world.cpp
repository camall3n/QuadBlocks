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
    piece(TETROMINO::T)
{
    for (int i=0; i<10; i++) {
        for (int j=0; j<22; j++) {
//            Block b = Block(BLOCK::COLOR::NUMBER[(i*11+j) % 7]);
//            Block b = Block(BLOCK::COLOR::NUMBER[randint(0, BLOCK::COLOR::MAX_NUMBER)]);
            Block b = Block(BLOCK::COLOR::BLACK);
            b.setPosition(glm::vec2(i+0.5, j+0.5));
//            b.setRotation(3.1416/4);
//            b.setOffset(glm::vec2(0.5, 0.5));
            blocks.push_back(b);
        }
    }
    
    std::cout << "nBlocks = " << Block::getNumBlockInstances() << std::endl;
    BOOST_FOREACH(Block block, blocks)
    {
        std::cout << block.position() << std::endl;
    }
    
    Block::useCamera(c);
    c.setPosition(glm::vec3(5,10,80));
    c.setTarget(glm::vec3(5,10,0));
    c.setOrientation(glm::vec3(0,1,0));
    c.makeActive();

    Block::useLight(light);
    light.setPosition(glm::vec3(5,40,35));
    light.setIntensity(glm::vec4(.5,.5,.5,1));
//    light.setAmbientIntensity(glm::vec4(.1,.1,.1,1));
    light.setAmbientIntensity(glm::vec4(0,0,0,1));
    light.setAttenuation(.15);
    
    piece.setPosition(glm::vec2(0,0));
    piece.setRotation(0);
}

void World::draw()
{
    BOOST_FOREACH(Block block, blocks) {
        block.draw();
    }
    piece.draw();
}