//
//  light.cpp
//  Tetris
//
//  Created by Cam Allen on 5/27/13.
//  Copyright (c) 2013 Cam Allen. All rights reserved.
//

#include "light.h"

#include <glm/gtc/type_ptr.hpp>
#include "utility.h"

Light::Light() :
_position(0,0,0)
{
    if (nLightInstances==0) {
        InitializeGLObjects();
    }
    nLightInstances++;
}

Light::Light(const Light& that)
{
    _position = that._position;
    if (nLightInstances==0) {
        InitializeGLObjects();
    }
    nLightInstances++;
}

Light& Light::operator=(const Light& that)
{
    _position = that._position;
    
    return *this;
}

Light::~Light()
{
    nLightInstances--;
    if (nLightInstances==0) {
        DestroyGLObjects();
    }
}

glm::vec3 Light::position() const
{
    return _glShared.worldLightPosition;
}
glm::vec4 Light::intensity() const
{
    return _glShared.lightIntensity;
}
glm::vec4 Light::ambientIntensity() const
{
    return _glShared.ambientIntensity;
}
float Light::attenuation() const
{
    return _glShared.lightAttenuation;
}

void Light::setPosition(glm::vec3 pos)
{
    if (_glShared.worldLightPosition != pos) {
        _glShared.worldLightPosition = pos;
        UpdateGLObjects();
    }
}
void Light::setIntensity(glm::vec4 color)
{
    if (_glShared.lightIntensity != color) {
        _glShared.lightIntensity = color;
        UpdateGLObjects();
    }
}
void Light::setAmbientIntensity(glm::vec4 color)
{
    if (_glShared.ambientIntensity != color) {
        _glShared.ambientIntensity = color;
        UpdateGLObjects();
    }
}
void Light::setAttenuation(float atten)
{
    if (_glShared.lightAttenuation != atten) {
        _glShared.lightAttenuation = atten;
        UpdateGLObjects();
    }
}

GLuint Light::getUBO()
{
    return _glObject.uniformBuffer;
}

size_t Light::getUBOSize()
{
    return sizeof(GLSharedUniforms);
}

size_t Light::getNumLightInstances()
{
    return nLightInstances;
}


void Light::InitializeGLObjects()
{
    _glProgram = LoadProgram( "basic.vert", "basic.frag" );
    checkError("during program compilation");
    
    _glObject.uniformBlockBinding = 0;
    glGenBuffers(1, &_glObject.uniformBuffer);
	glBindBuffer(GL_UNIFORM_BUFFER, _glObject.uniformBuffer);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(GLSharedUniforms), NULL, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    
    glBindBufferRange(GL_UNIFORM_BUFFER, _glObject.uniformBlockBinding, _glObject.uniformBuffer, 0, sizeof(GLSharedUniforms));
    GLuint uniformBlockIndex = glGetUniformBlockIndex(_glProgram, "TransformUniforms");
    glUniformBlockBinding(_glProgram, uniformBlockIndex, _glObject.uniformBlockBinding);
    
    checkError("during buffer creation");
}

void Light::UpdateGLObjects()
{
    glUseProgram(_glProgram);
    glBindBuffer(GL_UNIFORM_BUFFER, _glObject.uniformBuffer);
    
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(GLSharedUniforms), &_glShared);
    
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
    glUseProgram(0);
    
    checkError("while updating GL objects");
}

void Light::DestroyGLObjects()
{
    glDeleteBuffers(1, &_glObject.uniformBuffer);
    glDeleteProgram(_glProgram);
    
    checkError("during GL object destruction");
}


GLuint Light::_glProgram;
Light::GLObjects Light::_glObject;
Light::GLSharedUniforms Light::_glShared;
size_t Light::nLightInstances = 0;