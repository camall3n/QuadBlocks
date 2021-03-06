//
//  tetromino.cpp
//  QuadBlocks
//
//  Created by Cam Allen on 4/14/13.
//

#include "tetromino.h"
#include <boost/foreach.hpp>
#include <vector>

Tetromino::Tetromino(TETROMINO::TYPE type) :
    _type(type),
    _state(TETROMINO::ACTIVE),
    _position(glm::vec2(0,0)),
    _rotation(0)
{
    Block block[4];
    _color = getColor(_type);
    _center = getCenter(_type);
    _baseOffsets = getBaseOffsets(_type);

    for (int i=0; i<4; i++) {
        block[i] = Block(_color);
        block[i].setOffset(_baseOffsets[i]);
        block[i].setRotation(0);
        _blocks.push_back(block[i]);
    }
}

//Tetromino::Tetromino(const Tetromino& that) :
//    _type(that._type),
//    _state(that._state),
//    _color(that._color),
//    _position(that._position),
//    _center(that._center),
//    _rotation(that._rotation),
//    _blocks(that._blocks),
//    _baseOffsets(that._baseOffsets)
//{
//    
//}
//
//Tetromino& Tetromino::operator=(const Tetromino& that)
//{
//    
//}
//
//Tetromino::~Tetromino()
//{
//    
//}


bool Tetromino::operator==(const Tetromino& that)
{
    if ((_type == that._type) &&
        (_position == that._position) &&
        (_rotation == that._rotation) &&
        (_center == that._center) &&
        (_color == that._color) &&
        (_state == that._state))
        return true;
    
    return false;
}
bool Tetromino::operator!=(const Tetromino& that)
{
    if (*this == that)
        return false;
    
    return true;
}

void Tetromino::draw()
{
    BOOST_FOREACH(Block b, _blocks) {
        b.draw();
    }
}

glm::vec2 Tetromino::position()
{
    return glm::vec2(_position.x, _position.y);
}

float Tetromino::rotation()
{
    return fmod(_rotation+1, 1.0);//map angle onto [0, 1.0)
}

TETROMINO::TYPE Tetromino::type()
{
    return _type;
}

void Tetromino::setPosition(glm::vec2 pos)
{
//    if (_position != pos) {
        _position = pos;
        BOOST_FOREACH(Block& b, _blocks) {
            b.setPosition(_position + _center);
        }
//    }
}

void Tetromino::resetPosition()
{
    const glm::vec2 startingPos(4,20);

    if (this->type() == TETROMINO::TYPE::I) {
        this->setPosition(startingPos + glm::vec2(-1,0));
    }
    else {
        this->setPosition(startingPos);
    }
    this->setRotation(0);
}

void Tetromino::holdPosition()
{
    glm::vec2 holdPos = glm::vec2(-5,20-4);
    if (this->type() == TETROMINO::TYPE::O) {
        this->setPosition(holdPos+glm::vec2(1,1));
    }
    else if (this->type() == TETROMINO::TYPE::I) {
        this->setPosition(holdPos+glm::vec2(-1,0));
    }
    else {
        this->setPosition(holdPos);
    }
    this->setRotation(0);
}

void Tetromino::setRotation(float angle)
{
    angle = remainder(angle, 1.0);//map angle onto [-0.5, 0.5)
    if (_rotation != angle) {
        _rotation = angle;
        BOOST_FOREACH(Block& b, _blocks) {
            b.setRotation(_rotation);
        }
    }
}

void Tetromino::setState(TETROMINO::STATE state)
{
    _state = state;
    
    switch (_state) {
        case TETROMINO::GHOST:
            BOOST_FOREACH(Block& b, _blocks) {
                b.setColor(glm::mix(_color,BLOCK::COLOR::WHITE,0.75f));
                b.setAlpha(0.4);
            }
            break;
            
        case TETROMINO::UPCOMING:
        case TETROMINO::HOLDING:
        case TETROMINO::ACTIVE:
        default:
            
            BOOST_FOREACH(Block& b, _blocks) {
                b.setColor(_color);
                b.setAlpha(1.0);
            }
    }
}

std::list<Block> Tetromino::blocks()
{
    return _blocks;
}

glm::mat4 Tetromino::collisionSquare()
{
    glm::mat4 square(0);
    
    float offsetAdjustment = getMaxAbsOffset();
    
    int row, col;
    BOOST_FOREACH(glm::vec2 offset, _baseOffsets) {
        // Upside-down
        if (_rotation < -0.375 || _rotation >= 0.375) {
            row = floor(-offset.x + offsetAdjustment);
            col = floor(-offset.y + offsetAdjustment);
        }
        // Straight
        else if (_rotation >= -0.125 && _rotation < 0.125) {
            row = floor( offset.x + offsetAdjustment);
            col = floor( offset.y + offsetAdjustment);
        }
        // CCW rotation
        else if (_rotation < 0.0) {
            row = floor(-offset.y + offsetAdjustment);
            col = floor( offset.x + offsetAdjustment);
        }
        // CW rotation
        else {
            row = floor( offset.y + offsetAdjustment);
            col = floor(-offset.x + offsetAdjustment);
        }
        square[col][row] = 1;
    }
    
    return square;
}

int Tetromino::collisionSquareSize()
{
    switch (_type) {
        case TETROMINO::I:
            return 4;
        case TETROMINO::O:
            return 2;
        case TETROMINO::T:
            return 3;
        case TETROMINO::S:
            return 3;
        case TETROMINO::Z:
            return 3;
        case TETROMINO::J:
            return 3;
        case TETROMINO::L:
            return 3;
            
        default:
            return 0;
    }
}


glm::vec3 Tetromino::getColor(TETROMINO::TYPE type)
{
    glm::vec3 color(0);
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

float Tetromino::getMaxAbsOffset()
{
    float max = 0;
    BOOST_FOREACH(glm::vec2 offset, _baseOffsets) {
        float localMax = glm::max(glm::abs(offset.x), glm::abs(offset.y));
        if (max < localMax) {
            max = localMax;
        }
    }
    return max;
}

