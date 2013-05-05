//
//  block.cpp
//  Tetris
//
//  Created by Cam Allen on 4/14/13.
//  Copyright (c) 2013 Cam Allen. All rights reserved.
//

#include "block.h"

#include <algorithm>
#include <boost/foreach.hpp>
#include "ResourcePath.h"

Block::Block(glm::vec4 color) :
    _state(BLOCK::USER_CONTROL),
    _position(0,0),
    _offset(0,0),
    _rotation(0),
    _color(color),
    _vertexArray(NULL),
    _indexArray(NULL),
    _nVertices(0),
    _nIndices(0)
{
    _mesh.load( ResourcePath("block.xml") );
    _nVertices = _mesh.nVertices();
    _nIndices = _mesh.nIndices();
    
    _vertexArray = new float[_nVertices];
    _indexArray = new int[_nIndices];
    
    _mesh.fillVertexArray(_vertexArray);
    _mesh.fillIndexArray(_indexArray);
}

// Copy constructor
Block::Block(const Block& that) :
    _state(that._state),
    _position(that._position),
    _offset(that._offset),
    _rotation(that._rotation),
    _color(that._color),
    _mesh(that._mesh),
    _nVertices(that._nVertices),
    _nIndices(that._nIndices)
{
    _vertexArray = new float[_nVertices];
    _indexArray = new int[_nIndices];
    
    std::copy(that._vertexArray+0, that._vertexArray+_nVertices, _vertexArray);
    std::copy(that._indexArray+0, that._indexArray+_nIndices, _indexArray);
}

// Copy assignment operator
Block& Block::operator=(const Block& that)
{
    _state = that._state;
    _position = that._position;
    _offset = that._offset;
    _rotation = that._rotation;
    _color = that._color;
    _mesh = that._mesh;
    _nVertices = that._nVertices;
    _nIndices = that._nIndices;
    
    float* vertexArray = new float[_nVertices];
    std::copy(that._vertexArray+0, that._vertexArray+_nVertices, vertexArray);
    delete[] _vertexArray;
    _vertexArray = vertexArray;
    
    int* indexArray = new int[_nIndices];
    std::copy(that._indexArray+0, that._indexArray+_nIndices, indexArray);
    delete[] _indexArray;
    _indexArray = indexArray;
    
    return *this;
}

// Destructor
Block::~Block()
{
    delete[] _vertexArray;
    delete[] _indexArray;
}

