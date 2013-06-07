//
//  block.h
//  Tetris
//
//  Created by Cam Allen on 4/14/13.
//  Copyright (c) 2013 Cam Allen. All rights reserved.
//

#ifndef __Tetris__block__
#define __Tetris__block__

#include <iostream>
#include <GL/glew.h>
#include <GL/glfw.h>
#include <glm/glm.hpp>
#include "camera.h"
#include "light.h"
#include "mesh.h"

namespace BLOCK {
    namespace COLOR {
        const glm::vec4 CYAN   = glm::vec4(0.0, 0.9, 0.9, 1.0);
        const glm::vec4 YELLOW = glm::vec4(1.0, 0.9, 0.0, 1.0);
        const glm::vec4 PURPLE = glm::vec4(0.8, 0.1, 0.9, 1.0);
        const glm::vec4 GREEN  = glm::vec4(0.0, 0.9, 0.0, 1.0);
        const glm::vec4 RED    = glm::vec4(1.0, 0.1, 0.1, 1.0);
        const glm::vec4 BLUE   = glm::vec4(0.2, 0.2, 0.8, 1.0);
        const glm::vec4 ORANGE = glm::vec4(1.0, 0.5, 0.0, 1.0);
        const glm::vec4 BLACK  = glm::vec4(0.0, 0.0, 0.0, 1.0);
        const glm::vec4 GRAY   = glm::vec4(0.4, 0.4, 0.4, 1.0);
        const glm::vec4 WHITE  = glm::vec4(1.0, 1.0, 1.0, 1.0);
        
        const glm::vec4 NONE   = glm::vec4(0.0, 0.0, 0.0, 0.0);
        
        const glm::vec4 NUMBER[] = {
            CYAN, YELLOW, PURPLE, GREEN, RED, BLUE, ORANGE, //BLACK, GRAY, WHITE
        };
        const int MAX_NUMBER = sizeof(NUMBER)/sizeof(NUMBER[0]);
    };
    
    enum STATE {
        USER_CONTROL,
        GHOST,
        FALLING,
        FIXED,
        CLEARING,
    };
};

class Block
{
public:
    Block(glm::vec4 color=BLOCK::COLOR::WHITE); //Constructor
    Block(const Block& that);                   //Copy constructor
    Block& operator=(const Block& that);        //Copy assignment operator
    ~Block();                                   //Destructor

    void draw();
    
    glm::vec2 position() const;
    glm::vec2 offset() const;
    float rotation() const;
    void setPosition(glm::vec2 pos);
    void setPosition(glm::vec3 pos);
    void setOffset(glm::vec2 offset);
    void setRotation(float angle);// range [0,1)
    void setColor(glm::vec4 color);
    
    static size_t getNumBlockInstances();
    static void useCamera(Camera &camera);// Requires at least one block instance to exist
    static void useLight(Light &light);// Requires at least one block instance to exist
    
private:
    void InitializeGLObjects();// Called on creation of first block instance
    void DestroyGLObjects();// Called on destruction of last block instance
    
    // Instance member variables
    BLOCK::STATE _state;
    glm::vec3 _position;
    glm::vec3 _offset;
    float _rotation;// range [0,1)
    glm::vec4 _color;
    glm::mat4 _offsetModel;
    glm::mat4 _rotateToOrientation;
    glm::mat4 _translateToWorld;
    glm::mat4 _modelToWorld;
    
    // Types for static class variables
    struct GLAttributes {
        GLuint position;
        GLuint normal;
    };
    struct GLUniforms {
        GLuint diffuseColor;
        GLuint modelToWorld;
    };
    struct GLObjects {
        GLuint vertexArray;
        GLuint vertexBuffer;
        GLuint normalArray;
        GLuint normalBuffer;
        GLuint indexBuffer;
        GLuint transformUBO;
        GLuint transformBindingLocation;
        GLuint lightingUBO;
        GLuint lightingBindingLocation;
    };
    struct MeshDetails {
        int nVertices;
        int nNormals;
        int nIndices;
    };

    // Static class variables
    static GLuint _glProgram;
    static GLAttributes _glAttrib;
    static GLUniforms _glUniform;
    static GLObjects _glObject;
    static MeshDetails _mesh;
    
    static size_t nBlockInstances;
};

#endif /* defined(__Tetris__block__) */
