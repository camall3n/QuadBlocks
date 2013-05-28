//
//  mesh.h
//  Tetris
//
//  Created by Cam Allen on 5/4/13.
//  Copyright (c) 2013 Cam Allen. All rights reserved.
//

#ifndef __Tetris__mesh__
#define __Tetris__mesh__

#include <cstdlib>
#include <string>
#include <list>
#include <GL/glew.h>

class Mesh
{
public:
    Mesh();
    ~Mesh();
    
    void load(const std::string filename);
    void save(const std::string filename);
    
    size_t nVertices();
    size_t nNormals();
    size_t nIndices();
    
    void fillVertexArray(float* vertexArray);
    void fillNormalArray(float* normalArray);
    void fillIndexArray(GLshort* indexArray);
    
private:
    std::string name;
    std::list<float> vertices;
    std::list<float> normals;
    std::list<int> indices;
};

#endif /* defined(__Tetris__mesh__) */
