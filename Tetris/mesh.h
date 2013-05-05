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
#include <glm/glm.hpp>

class Mesh
{
public:
    Mesh();
    ~Mesh();
    
    void load(const std::string filename);
    void save(const std::string filename);
//    
//    size_t nVertices();
//    size_t nIndices();
    
//    void fillVertexArray(float (&vertices)[], size_t nVertices);
//    void fillIndexArray(int (&indices)[], size_t nIndices);
    
private:
    std::string name;
    std::list<float> vertices;
    std::list<int> indices;
};

#endif /* defined(__Tetris__mesh__) */
