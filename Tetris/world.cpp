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

World::World()
{
    for (int i=0; i<10; i++) {
        for (int j=0; j<22; j++) {
//            Block b = Block(BLOCK::COLOR::NUMBER[(i*11+j) % 7]);
            Block b = Block(BLOCK::COLOR::NUMBER[randint(0, BLOCK::COLOR::MAX_NUMBER)]);
            b.setPosition(glm::vec2(i, j));
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
    light.setPosition(glm::vec3(5,40,25));
    light.setIntensity(glm::vec4(.5,.5,.5,1));
//    light.setAmbientIntensity(glm::vec4(.1,.1,.1,1));
    light.setAmbientIntensity(glm::vec4(0,0,0,1));
    light.setAttenuation(.15);
}

void World::draw()
{
    BOOST_FOREACH(Block block, blocks) {
        block.draw();
    }
}