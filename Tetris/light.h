//
//  light.h
//  Tetris
//
//  Created by Cam Allen on 5/27/13.
//  Copyright (c) 2013 Cam Allen. All rights reserved.
//

#ifndef __Tetris__light__
#define __Tetris__light__

#include <GL/glew.h>
#include <GL/glfw.h>
#include <glm/glm.hpp>
//#include "model.h"

class Light
{
public:
    Light();
    Light(const Light& that);
    Light& operator=(const Light& that);
    ~Light();
    
    glm::vec3 position() const;
    glm::vec4 intensity() const;
    glm::vec4 ambientIntensity() const;
    float attenuation() const;
    
    void setPosition(glm::vec3 pos);
    void setIntensity(glm::vec4 color);
    void setAmbientIntensity(glm::vec4 color);
    void setAttenuation(float atten);
    
    GLuint getUBO();
    size_t getUBOSize();
    static size_t getNumLightInstances();
    
private:
    void InitializeGLObjects();
    void UpdateGLObjects();
    void DestroyGLObjects();
    
    glm::vec3 _position;
    
    struct GLObjects {
        GLuint uniformBuffer;
        GLuint uniformBlockBinding;
    };
    struct GLSharedUniforms {
        glm::vec3 worldLightPosition = glm::vec3(0.f);
        glm::vec4 lightIntensity = glm::vec4(0.f);
        glm::vec4 ambientIntensity = glm::vec4(0.f);
        float lightAttenuation = 0.f;
    };
    
    static GLuint _glProgram;
    static GLObjects _glObject;
    static GLSharedUniforms _glShared;
    
    static size_t nLightInstances;
    
};
#endif /* defined(__Tetris__light__) */
