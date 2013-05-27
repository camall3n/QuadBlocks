//
//  camera.cpp
//  Tetris
//
//  Created by Cam Allen on 5/19/13.
//  Copyright (c) 2013 Cam Allen. All rights reserved.
//

#include "camera.h"

#include <glm/gtc/type_ptr.hpp>
#include "utility.h"

Camera::Camera() :
    _position(0,0,1),
    _target(0,0,0),
    _upDir(0,1,0)
{
    if (nCameraInstances==0) {
        InitializeGLObjects();
    }
    nCameraInstances++;
}

Camera::Camera(const Camera& that)
{
    _position = that._position;
    _target = that._target;
    _upDir = that._upDir;
    if (nCameraInstances==0) {
        InitializeGLObjects();
    }
    nCameraInstances++;
}

Camera& Camera::operator=(const Camera& that)
{
    _position = that._position;
    _target = that._target;
    _upDir = that._upDir;
    
    return *this;
}

Camera::~Camera()
{
    nCameraInstances--;
    if (nCameraInstances==0) {
        DestroyGLObjects();
    }
}

void Camera::makeActive()
{
    if (this != _activeCamera) {
        _activeCamera = this;
        UpdateGLObjects();
    }
}

glm::vec3 Camera::position() const
{
    return _position;
}
glm::vec3 Camera::target() const
{
    return _target;
}
glm::vec3 Camera::orientation() const
{
    return _upDir;
}

void Camera::setPosition(glm::vec3 pos)
{
    if (_position != pos) {
        _position = pos;
        if (this == _activeCamera) {
            UpdateGLObjects();
        }
    }
}
void Camera::setTarget(glm::vec3 target)
{
    if (_target != target) {
        _target = target;
        if (this == _activeCamera) {
            UpdateGLObjects();
        }
    }
}
void Camera::setOrientation(glm::vec3 up)
{
    if (_upDir != up) {
        _upDir = up;
        if (this == _activeCamera) {
            UpdateGLObjects();
        }
    }
}

GLuint Camera::getUBO()
{
    return _glObject.uniformBuffer;
}

size_t Camera::getNumCameraInstances()
{
    return nCameraInstances;
}


void Camera::InitializeGLObjects()
{
    _glProgram = LoadProgram( "basic.vert", "basic.frag" );
    checkError("during program compilation");
    
    _glObject.uniformBlockBinding = 0;
    glGenBuffers(1, &_glObject.uniformBuffer);
	glBindBuffer(GL_UNIFORM_BUFFER, _glObject.uniformBuffer);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(GLSharedUniforms), NULL, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    
    glBindBufferRange(GL_UNIFORM_BUFFER, _glObject.uniformBlockBinding, _glObject.uniformBuffer, 0, sizeof(glm::mat4));
    GLuint uniformBlockIndex = glGetUniformBlockIndex(_glProgram, "TransformUniforms");
    glUniformBlockBinding(_glProgram, uniformBlockIndex, _glObject.uniformBlockBinding);
    
    checkError("during buffer creation");
}

void Camera::UpdateGLObjects()
{
    glm::mat4 worldToCameraMatrix = CalcLookAtMatrix();
    glm::mat4 cameraToClipMatrix = PerspectiveMatrix();
    
    _glShared.worldToClipMatrix = cameraToClipMatrix * worldToCameraMatrix;

    
    glUseProgram(_glProgram);
    glBindBuffer(GL_UNIFORM_BUFFER, _glObject.uniformBuffer);

	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(GLSharedUniforms), &_glShared);

	glBindBuffer(GL_UNIFORM_BUFFER, 0);
    glUseProgram(0);

    checkError("while updating GL objects");
}

void Camera::DestroyGLObjects()
{
    glDeleteBuffers(1, &_glObject.uniformBuffer);
    glDeleteProgram(_glProgram);
    
    checkError("during GL object destruction");
}

glm::mat4 Camera::CalcLookAtMatrix()
{
    glm::mat4 modelToCamera(1.f);
    
    glm::vec3 lookDir = glm::normalize(_target - _position);
	glm::vec3 upDir = glm::normalize(_upDir);
	glm::vec3 rightDir = glm::normalize(glm::cross(lookDir, upDir));
	glm::vec3 perpUpDir = glm::cross(rightDir, lookDir);
    
    glm::mat4 rotMat(1.f);
    std::cout << "position = " << _position << endl;
    std::cout << "target = " << _target << endl;
    std::cout << "upDir = " << _upDir << endl;
    rotMat[0] = glm::vec4(rightDir, 0.f);
    rotMat[1] = glm::vec4(perpUpDir, 0.f);
    rotMat[2] = glm::vec4(-lookDir, 0.f);
    rotMat = glm::transpose(rotMat);
    
    std::cout << "rotMat=" << std::endl;
    std::cout << rotMat << std::endl;
    
    glm::mat4 transMat(1.f);
    transMat[3] = glm::vec4(-_position, 1.f);
    
    std::cout << "transMat=" << std::endl;
    std::cout << transMat << std::endl;
    
    return rotMat * transMat;
}

float CalcFrustumScale(float fFovDeg)
{
	const float degToRad = 3.14159f * 2.0f / 360.0f;
	float fFovRad = fFovDeg * degToRad;
	return 1.0f / tan(fFovRad / 2.0f);
}

glm::mat4 Camera::PerspectiveMatrix()
{
    glm::mat4 perspectiveMat(1.f);
    
    float viewAngle = 30;//degrees
    float frustumScale = CalcFrustumScale(viewAngle);
    float zNear = 1.f;
    float zFar = 45.f;
    
    perspectiveMat[0].x = frustumScale;
    perspectiveMat[1].y = frustumScale;
    perspectiveMat[2].z = (zFar+zNear)/(zNear-zFar);
    perspectiveMat[2].w = -1.f;
    perspectiveMat[3].z = 2*zFar*zNear/(zNear-zFar);
    
    return perspectiveMat;
}


GLuint Camera::_glProgram;
Camera::GLObjects Camera::_glObject;
Camera::GLSharedUniforms Camera::_glShared;
Camera* Camera::_activeCamera = NULL;
size_t Camera::nCameraInstances = 0;