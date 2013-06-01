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

class Light
{
public:
    Light();
    Light(const Light& that);
    Light& operator=(const Light& that);
    ~Light();
    
    void makeActive();
    
    glm::vec3 position() const;
    glm::vec3 lightColor() const;
    glm::vec3 darkColor() const;
    float attenuation() const;
    
    void setPosition(glm::vec3 pos);
    void setLightColor(glm::vec3 color);
    void setDarkColor(glm::vec3 color);
    void setAttenuation(float atten);
    
    GLuint getUBO();
    size_t getUBOSize();
    static size_t getNumLightInstances();
    static Light* getActiveLight();
    
private:
    void InitializeGLObjects();
    void UpdateGLObjects();
    void DestroyGLObjects();
    
    //Local variables
    glm::vec3 _position;
    glm::vec3 _lightColor;
    glm::vec3 _darkColor;
    float _attenuation;
    
    //Shared variables
    struct GLObjects {
        GLuint uniformBuffer;
        GLuint uniformBlockBinding;
    };
    struct GLSharedUniforms {
        // Note: Even though the GLSL counterparts are vec3s, they are padded to
        //       be the same size as vec4s. In order to match layout(std140),
        //       you need to pad these variables out to glm::vec4s.
        glm::vec4 worldLightPosition = glm::vec4(0.f);
        glm::vec4 lightColor = glm::vec4(0.f);
        glm::vec4 darkColor = glm::vec4(0.f);
        float lightAttenuation = 0.f;
    };
    
    static GLuint _glProgram;
    static GLObjects _glObject;
    static GLSharedUniforms _glShared;
    static Light* _activeLight;
    
    static size_t nLightInstances;
    
};
#endif /* defined(__Tetris__light__) */
