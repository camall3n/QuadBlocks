//
//  kicktable.h
//  QuadBlocks
//
//  Created by Cam Allen on 7/13/13.
//  Copyright (c) 2013 Cam Allen. All rights reserved.
//

#ifndef __QuadBlocks__kicktable__
#define __QuadBlocks__kicktable__

#include <GL/glew.h>
#include <GL/glfw.h>
#include <glm/glm.hpp>
using glm::vec2;

#include <vector>
using std::vector;

class KickTable {
    // 3D array
    // dim 0 is the initial rotation state
    // dim 1 is the rotation direction (0 CCW, 1 CW)
    // dim 2 is the different kick offsets
    vector< vector< vector<vec2> > > main_table;
    vector< vector< vector<vec2> > > i_table;

public:
    KickTable();
    
    vec2 getOffset(int fromState, int dir, int kicks);
    vec2 getIOffset(int fromState, int dir, int kicks);
};

#endif /* defined(__QuadBlocks__kicktable__) */
