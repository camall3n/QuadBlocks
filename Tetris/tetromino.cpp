//
//  tetromino.cpp
//  Tetris
//
//  Created by Cam Allen on 4/14/13.
//

#include "tetromino.h"
#include <boost/foreach.hpp>
#include <vector>

Tetromino::Tetromino(TETROMINO::TYPE type) :
    _type(type),
    _state(TETROMINO::ACTIVE),
    _currentCommand(TETROMINO::NONE),
    _position(glm::vec2(0,0)),
    _rotation(0)
{
    Block block[4];
    _color = getColor(_type);
    _center = getCenter(_type);
    std::vector<glm::vec2> base_offsets = getBaseOffsets(_type);

    for (int i=0; i<4; i++) {
        block[i] = Block(_color);
        block[i].setOffset(base_offsets[i]);
        block[i].setRotation(0);
        _blocksList.push_back(block[i]);
    }
    
}

void Tetromino::draw()
{
    BOOST_FOREACH(Block b, _blocksList) {
        b.draw();
    }
}

void Tetromino::setPosition(glm::vec2 pos)
{
//    if (_position != pos) {
        _position = pos;
        BOOST_FOREACH(Block& b, _blocksList) {
            b.setPosition(_position + _center);
        }
//    }
}

void Tetromino::setRotation(float angle)
{
    if (_rotation != angle) {
        _rotation = angle;
        BOOST_FOREACH(Block& b, _blocksList) {
            b.setRotation(_rotation);
        }
    }
}


glm::vec4 Tetromino::getColor(TETROMINO::TYPE type)
{
    glm::vec4 color(0);
    switch (type) {
        case TETROMINO::I:
            color = BLOCK::COLOR::CYAN; break;
        case TETROMINO::O:
            color = BLOCK::COLOR::YELLOW; break;
        case TETROMINO::T:
            color = BLOCK::COLOR::PURPLE; break;
        case TETROMINO::S:
            color = BLOCK::COLOR::GREEN; break;
        case TETROMINO::Z:
            color = BLOCK::COLOR::RED; break;
        case TETROMINO::J:
            color = BLOCK::COLOR::BLUE; break;
        case TETROMINO::L:
            color = BLOCK::COLOR::ORANGE; break;
            
        default:
            color = BLOCK::COLOR::BLACK; break;
    }
    return color;
}

glm::vec2 Tetromino::getCenter(TETROMINO::TYPE type)
{
    glm::vec2 center(0,0);
    switch (type) {
        case TETROMINO::I:
            center = glm::vec2(2,2); break;
        case TETROMINO::O:
            center = glm::vec2(1,1); break;
        case TETROMINO::T:
            center = glm::vec2(1.5,1.5); break;
        case TETROMINO::S:
            center = glm::vec2(1.5,1.5); break;
        case TETROMINO::Z:
            center = glm::vec2(1.5,1.5); break;
        case TETROMINO::J:
            center = glm::vec2(1.5,1.5); break;
        case TETROMINO::L:
            center = glm::vec2(1.5,1.5); break;
            
        default:
            center = glm::vec2(0,0); break;
    }
    return center;
}

std::vector<glm::vec2> Tetromino::getBaseOffsets(TETROMINO::TYPE type)
{
    std::vector<glm::vec2> base_offsets(4);
    switch (type) {
        case TETROMINO::I:
            base_offsets[0] = glm::vec2(-1.5, 0.5);
            base_offsets[1] = glm::vec2(-0.5, 0.5);
            base_offsets[2] = glm::vec2( 0.5, 0.5);
            base_offsets[3] = glm::vec2( 1.5, 0.5);
            break;
        case TETROMINO::O:
            base_offsets[0] = glm::vec2(-0.5,  0.5);
            base_offsets[1] = glm::vec2( 0.5,  0.5);
            base_offsets[2] = glm::vec2(-0.5, -0.5);
            base_offsets[3] = glm::vec2( 0.5, -0.5);
            break;
        case TETROMINO::T:
            base_offsets[0] = glm::vec2(  0.0, 0.0);
            base_offsets[1] = glm::vec2(  0.0, 1.0);
            base_offsets[2] = glm::vec2( -1.0, 0.0);
            base_offsets[3] = glm::vec2(  1.0, 0.0);
            break;
        case TETROMINO::S:
            base_offsets[0] = glm::vec2( 0.0, 0.0);
            base_offsets[1] = glm::vec2( 0.0, 1.0);
            base_offsets[2] = glm::vec2( 1.0, 1.0);
            base_offsets[3] = glm::vec2(-1.0, 0.0);
            break;
        case TETROMINO::Z:
            base_offsets[0] = glm::vec2( 0.0, 0.0);
            base_offsets[1] = glm::vec2(-1.0, 1.0);
            base_offsets[2] = glm::vec2( 0.0, 1.0);
            base_offsets[3] = glm::vec2( 1.0, 0.0);
            break;
        case TETROMINO::J:
            base_offsets[0] = glm::vec2( 0.0, 0.0);
            base_offsets[1] = glm::vec2(-1.0, 1.0);
            base_offsets[2] = glm::vec2(-1.0, 0.0);
            base_offsets[3] = glm::vec2( 1.0, 0.0);
            break;
        case TETROMINO::L:
            base_offsets[0] = glm::vec2( 0.0, 0.0);
            base_offsets[1] = glm::vec2( 1.0, 1.0);
            base_offsets[2] = glm::vec2(-1.0, 0.0);
            base_offsets[3] = glm::vec2( 1.0, 0.0);
            break;
            
        default:
            base_offsets[0] = glm::vec2(-1.5,  1.5);
            base_offsets[1] = glm::vec2( 1.5,  1.5);
            base_offsets[2] = glm::vec2( 1.5, -1.5);
            base_offsets[3] = glm::vec2(-1.5, -1.5);
            break;
    }
    return base_offsets;
}