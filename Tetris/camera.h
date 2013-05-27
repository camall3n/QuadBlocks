//
//  camera.h
//  Tetris
//
//  Created by Cam Allen on 5/19/13.
//  Copyright (c) 2013 Cam Allen. All rights reserved.
//

#ifndef __Tetris__camera__
#define __Tetris__camera__

#include <GL/glew.h>
#include <GL/glfw.h>
#include <glm/glm.hpp>
//#include "model.h"

class Camera
{
public:
    Camera();
    Camera(const Camera& that);
    Camera& operator=(const Camera& that);
    ~Camera();
    
    void makeActive();
    
    glm::vec3 position() const;
    glm::vec3 target() const;
    glm::vec3 orientation() const;
    void setPosition(glm::vec3 pos);
    void setTarget(glm::vec3 target);
    void setOrientation(glm::vec3 up);
    
    GLuint getUBO();
    static size_t getNumCameraInstances();
    
private:
    void InitializeGLObjects();
    void UpdateGLObjects();
    void DestroyGLObjects();
    
    glm::mat4 CalcLookAtMatrix();
    glm::mat4 PerspectiveMatrix();
    
    glm::vec3 _position;
    glm::vec3 _target;
    glm::vec3 _upDir;
    
    glm::mat4 _worldToCameraMatrix;
    glm::mat4 _cameraToClipMatrix;

    struct GLObjects {
        GLuint uniformBuffer;
        GLuint uniformBlockBinding;
    };
    struct GLSharedUniforms {
        glm::mat4 worldToClipMatrix = glm::mat4(1.0);
//        glm::mat4 cameraToClipMatrix = glm::mat4(1.0);
    };
    
    static GLuint _glProgram;
    static GLObjects _glObject;
    static GLSharedUniforms _glShared;
    static Camera* _activeCamera;
    
    static size_t nCameraInstances;
    
};
#endif /* defined(__Tetris__camera__) */
