//
//  block.cpp
//  Tetris
//
//  Created by Cam Allen on 4/14/13.
//  Copyright (c) 2013 Cam Allen. All rights reserved.
//

#include "block.h"

#include <glm/gtc/type_ptr.hpp>
#include "ResourcePath.h"
#include "utility.h"
#include "math.h"

Block::Block(glm::vec3 color) :
    _state(BLOCK::USER_CONTROL),
    _position(0,0,0),
    _offset(0,0,0),
    _rotation(0),
    _scale(1),
    _color(color),
    _alpha(1.0),
    _offsetModel(1.f),
    _scaleModel(1.f),
    _rotateToOrientation(1.f),
    _translateToWorld(1.f),
    _modelToWorld(1.f)
{
    if (nBlockInstances==0) {
        InitializeGLObjects();
    }
    nBlockInstances++;
}

// Copy constructor
Block::Block(const Block& that) :
    _state(that._state),
    _position(that._position),
    _offset(that._offset),
    _rotation(that._rotation),
    _scale(that._scale),
    _color(that._color),
    _alpha(that._alpha),
    _offsetModel(that._offsetModel),
    _scaleModel(that._scaleModel),
    _rotateToOrientation(that._rotateToOrientation),
    _translateToWorld(that._translateToWorld),
    _modelToWorld(that._modelToWorld)
{
    if (nBlockInstances==0) {
        InitializeGLObjects();
    }
    nBlockInstances++;
}

// Copy assignment operator
Block& Block::operator=(const Block& that)
{
    _state = that._state;
    _position = that._position;
    _offset = that._offset;
    _rotation = that._rotation;
    _scale = that._scale;
    _color = that._color;
    _alpha = that._alpha;
    _offsetModel = that._offsetModel;
    _scaleModel = that._scaleModel;
    _rotateToOrientation = that._rotateToOrientation;
    _translateToWorld = that._translateToWorld;
    _modelToWorld = that._modelToWorld;
    
    return *this;
}

// Destructor
Block::~Block()
{
    nBlockInstances--;
    if (nBlockInstances == 0) {
        DestroyGLObjects();
    }
}

void Block::draw()
{
    _modelToWorld = _translateToWorld * _rotateToOrientation * _offsetModel * _scaleModel;
    glm::vec4 color = glm::vec4(_color, _alpha);

    glUseProgram(_glProgram);
    
    glBindVertexArray(_glObject.vertexArray);
    {
        glUniform4fv(_glUniform.diffuseColor, 1, glm::value_ptr(color));
        glUniformMatrix4fv(_glUniform.modelToWorld, 1, GL_FALSE, glm::value_ptr(_modelToWorld));
        
        glDrawElements(GL_TRIANGLES, _mesh.nIndices, GL_UNSIGNED_SHORT, 0);
    }
    glBindVertexArray(0);
    
    glUseProgram(0);
    
    checkError("during draw");
}

glm::vec2 Block::position() const
{
    return glm::vec2(_position.x, _position.y);
}
glm::vec2 Block::offset() const
{
    return glm::vec2(_offset.x, _offset.y);
}
float Block::rotation() const
{
    return _rotation;
}

void Block::setPosition(glm::vec2 pos)
{
    glm::vec3 xyz = glm::vec3(pos, 0.f);
    if (_position != xyz) {
        _position = xyz;
        _translateToWorld = glm::mat4(1.f);
        _translateToWorld[3] = glm::vec4(_position, 1.f);
    }
}
void Block::setPosition(glm::vec3 pos)
{
    glm::vec3 xyz = pos;
    if (_position != xyz) {
        _position = xyz;
        _translateToWorld = glm::mat4(1.f);
        _translateToWorld[3] = glm::vec4(_position, 1.f);
    }
}
void Block::setOffset(glm::vec2 offset)
{
    glm::vec3 xyz = glm::vec3(offset, 0.f);
    if (_offset != xyz) {
        _offset = xyz;
        _offsetModel = glm::mat4(1.f);
        _offsetModel[3] = glm::vec4(_offset, 1.f);
    }
}
void Block::setRotation(float angle)
{
    if (_rotation != angle) {
        _rotation = angle;
        float theta = 2*glm::pi<float>()*angle;
        _rotateToOrientation = glm::mat4(1.f);
        _rotateToOrientation[0].x =  cos(theta);
        _rotateToOrientation[0].y = -sin(theta);
        _rotateToOrientation[1].x =  sin(theta);
        _rotateToOrientation[1].y =  cos(theta);
    }
}
void Block::setScale(float scale)
{
    if (_scale != scale) {
        _scale = scale;
        _scaleModel = glm::mat4(1.f);
        _scaleModel[0].x = _scale;
        _scaleModel[1].y = _scale;
    }
}

void Block::setColor(glm::vec3 color)
{
    _color = color;
}

void Block::setAlpha(float alpha)
{
    _alpha = alpha;
}

size_t Block::getNumBlockInstances()
{
    return nBlockInstances;
}

void Block::useCamera(Camera &camera)
{
    if (nBlockInstances==0) {
        fprintf(stderr, "Error: useCamera() called while there were no Block insances.\n");
    }
    if (Camera::getNumCameraInstances() == 0) {
        fprintf(stderr, "Error: useCamera() called while there were no Camera insances.\n");
    }
    _glObject.transformBindingLocation = 0;
    GLuint uniformBlockIndex = glGetUniformBlockIndex(_glProgram, "TransformUniforms");
    glUniformBlockBinding(_glProgram, uniformBlockIndex, _glObject.transformBindingLocation);

    _glObject.transformUBO = camera.getUBO();
    glBindBufferRange(GL_UNIFORM_BUFFER, _glObject.transformBindingLocation, _glObject.transformUBO, 0, camera.getUBOSize());
    
    checkError("while setting up the camera");
}

void Block::useLight(Light &light)
{
    if (nBlockInstances==0) {
        fprintf(stderr, "Error: useCamera() called while there were no Block insances.\n");
    }
    if (Camera::getNumCameraInstances() == 0) {
        fprintf(stderr, "Error: useCamera() called while there were no Camera insances.\n");
    }
    _glObject.lightingBindingLocation = 1;
    GLuint uniformBlockIndex = glGetUniformBlockIndex(_glProgram, "LightingUniforms");
    glUniformBlockBinding(_glProgram, uniformBlockIndex, _glObject.lightingBindingLocation);
    
    _glObject.lightingUBO = light.getUBO();
    glBindBufferRange(GL_UNIFORM_BUFFER, _glObject.lightingBindingLocation, _glObject.lightingUBO, 0, light.getUBOSize());
    
    checkError("while setting up the light");
}

void Block::InitializeGLObjects()
{
    Mesh mesh;
    mesh.load( ResourcePath("block.xml") );
    
    _mesh.nVertices = (int) mesh.nVertices();
    _mesh.nNormals = (int) mesh.nNormals();
    _mesh.nIndices = (int) mesh.nIndices();
    
    float* vertexData = new float[_mesh.nVertices];
    float* normalData = new float[_mesh.nNormals];
    GLshort* indexData = new GLshort[_mesh.nIndices];
    
    mesh.fillVertexArray(vertexData);
    mesh.fillNormalArray(normalData);
    mesh.fillIndexArray(indexData);
    
    // Create and compile program
    _glProgram = LoadProgram( "basic.vert", "basic.frag" );
    checkError("during program compilation");
    
    _glAttrib.position = glGetAttribLocation(_glProgram, "position");
    _glAttrib.normal = glGetAttribLocation(_glProgram, "normal");

	_glUniform.diffuseColor = glGetUniformLocation(_glProgram, "diffuseColor");
    _glUniform.modelToWorld = glGetUniformLocation(_glProgram, "modelToWorldMatrix");
    checkError("during location finding");
    
    // Generate and fill GL buffers
    GLsizeiptr nVertexBytes = sizeof(vertexData[0]) * _mesh.nVertices;
    glGenBuffers(1, &_glObject.vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, _glObject.vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, nVertexBytes, vertexData, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    GLsizeiptr nNormalBytes = sizeof(normalData[0]) * _mesh.nNormals;
    glGenBuffers(1, &_glObject.normalBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, _glObject.normalBuffer);
	glBufferData(GL_ARRAY_BUFFER, nNormalBytes, normalData, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    GLsizeiptr nIndexBytes = sizeof(indexData[0]) * _mesh.nIndices;
    glGenBuffers(1, &_glObject.indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _glObject.indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, nIndexBytes, indexData, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);    
    checkError("during buffer creation");
    
    // Generate and configure VAO for the GL buffers
    glGenVertexArrays(1, &_glObject.vertexArray);
	glBindVertexArray(_glObject.vertexArray);
    
    glBindBuffer(GL_ARRAY_BUFFER, _glObject.vertexBuffer);
    glEnableVertexAttribArray(_glAttrib.position);
    glVertexAttribPointer(_glAttrib.position, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, _glObject.normalBuffer);
    glEnableVertexAttribArray(_glAttrib.normal);
    glVertexAttribPointer(_glAttrib.normal, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _glObject.indexBuffer);
    
    glBindVertexArray(0);
    checkError("during VAO creation");
        
    delete[] vertexData;
    delete[] indexData;
}

void Block::DestroyGLObjects()
{
    glDeleteBuffers(1, &_glObject.vertexBuffer);
    glDeleteBuffers(1, &_glObject.indexBuffer);
    glDeleteProgram(_glProgram);
    
    checkError("during GL object destruction");
}


GLuint Block::_glProgram;
Block::GLAttributes Block::_glAttrib;
Block::GLUniforms Block::_glUniform;
Block::GLObjects Block::_glObject;
Block::MeshDetails Block::_mesh;
size_t Block::nBlockInstances;
