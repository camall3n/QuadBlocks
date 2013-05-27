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
#include "mesh.h"
#include "model.h"

namespace BLOCK {
    namespace COLOR {
        const glm::vec4 CYAN   = glm::vec4(0.f, .9f, .9f, 1.f);
        const glm::vec4 YELLOW = glm::vec4(1.f, .9f, 0.f, 1.f);
        const glm::vec4 PURPLE = glm::vec4(.7f, 0.f, 1.f, 1.f);
        const glm::vec4 GREEN  = glm::vec4(0.f, .9f, 0.f, 1.f);
        const glm::vec4 RED    = glm::vec4(.9f, 0.f, 0.f, 1.f);
        const glm::vec4 BLUE   = glm::vec4(0.f, 0.f, 1.f, 1.f);
        const glm::vec4 ORANGE = glm::vec4(1.f, .5f, 0.f, 1.f);
        const glm::vec4 GRAY   = glm::vec4(.7f, .7f, .7f, 1.f);
        const glm::vec4 WHITE  = glm::vec4(9.f, 9.f, 9.f, 1.f);
        
        const glm::vec4 NONE   = glm::vec4(0.f, 0.f, 0.f, 0.f);
        
        const glm::vec4 NUMBER[] = {
            CYAN, YELLOW, PURPLE, GREEN, RED, BLUE, ORANGE, GRAY, WHITE
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

class Block : public Model
{
public:
    Block(glm::vec4 color=BLOCK::COLOR::WHITE); //Constructor
    Block(const Block& that);                   //Copy constructor
    Block& operator=(const Block& that);        //Copy assignment operator
    ~Block();                                   //Destructor

    void draw();
    
    glm::vec2 position() const;
    void setPosition(glm::vec2 pos);
    
    static size_t getNumBlockInstances();
    static void useCamera(Camera &c);// Requires at least one block instance to exist
    
private:
    void InitializeGLObjects();// Called on creation of first block instance
    void DestroyGLObjects();// Called on destruction of last block instance
    
    // Instance member variables
    BLOCK::STATE _state;
    glm::vec3 _position;
    glm::vec2 _offset;
    float _rotation;
    glm::vec4 _color;
    glm::mat4 _modelToWorld;
    
    // Types for static class variables
    struct GLAttributes {
        GLuint position;
    };
    struct GLUniforms {
        GLuint diffuseColor;
        GLuint modelToWorld;
    };
    struct GLObjects {
        GLuint vertexArray;
        GLuint vertexBuffer;
        GLuint indexBuffer;
        GLuint uniformBuffer;
        GLuint uniformBindingLocation;
    };
    struct MeshDetails {
        int nVertices;
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
