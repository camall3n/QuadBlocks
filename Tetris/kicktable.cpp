//
//  kicktable.cpp
//  Tetris
//
//  Created by Cam Allen on 7/13/13.
//  Copyright (c) 2013 Cam Allen. All rights reserved.
//

#include "kicktable.h"
#include <iostream>

KickTable::KickTable() :
    // Initialize tables as 4x2x5 3D arrays of type glm::vec2
    main_table(4, vector<vector<vec2>>(2, vector<vec2>(5, vec2(0,0) ) ) ),
    i_table   (4, vector<vector<vec2>>(2, vector<vec2>(5, vec2(0,0) ) ) )
{
    
    //--------------------------------------------------------------
    // main_table (implements SRS for J, L, S, T, Z)
    //--------------------------------------------------------------
    for (int i=0; i<4; i++) {
        main_table[i][0][0] = vec2(+0,+0);
        main_table[i][1][0] = vec2(+0,+0);
    }
    
    // 0 from 1  (1 -> CCW)
    main_table[1][0][1] = vec2(+1,+0);
    main_table[1][0][2] = vec2(+1,-1);
    main_table[1][0][3] = vec2(+0,+2);
    main_table[1][0][4] = vec2(+1,+2);
    
    // 1 from 0 (0 -> CW)
    main_table[0][1][1] = vec2(-1,+0);
    main_table[0][1][2] = vec2(-1,+1);
    main_table[0][1][3] = vec2(+0,-2);
    main_table[0][1][4] = vec2(-1,-2);
    
    
    // 1 from 2  (2 -> CCW)
    main_table[2][0][1] = vec2(-1,+0);
    main_table[2][0][2] = vec2(-1,+1);
    main_table[2][0][3] = vec2(+0,-2);
    main_table[2][0][4] = vec2(-1,-2);
    
    // 2 from 1  (1 -> CW)
    main_table[1][1][1] = vec2(+1,+0);
    main_table[1][1][2] = vec2(+1,-1);
    main_table[1][1][3] = vec2(+0,+2);
    main_table[1][1][4] = vec2(+1,+2);
    
    
    // 2 from 3  (3 -> CCW)
    main_table[3][0][1] = vec2(-1,+0);
    main_table[3][0][2] = vec2(-1,-1);
    main_table[3][0][3] = vec2(+0,+2);
    main_table[3][0][4] = vec2(-1,+2);
    
    // 3 from 2  (2 -> CW)
    main_table[2][1][1] = vec2(+1,+0);
    main_table[2][1][2] = vec2(+1,+1);
    main_table[2][1][3] = vec2(+0,-2);
    main_table[2][1][4] = vec2(+1,-2);
    
    
    // 3 from 0  (0 -> CCW)
    main_table[0][0][1] = vec2(+1,+0);
    main_table[0][0][2] = vec2(+1,+1);
    main_table[0][0][3] = vec2(+0,-2);
    main_table[0][0][4] = vec2(+1,-2);
    
    // 0 from 3  (3 -> CW)
    main_table[3][1][1] = vec2(-1,+0);
    main_table[3][1][2] = vec2(-1,-1);
    main_table[3][1][3] = vec2(+0,+2);
    main_table[3][1][4] = vec2(-1,+2);
    
    //--------------------------------------------------------------
    // i_table (uses the Arika SRS variation for I)
    //--------------------------------------------------------------
    for (int i=0; i<4; i++) {
        i_table[i][0][0] = vec2(+0,+0);
        i_table[i][1][0] = vec2(+0,+0);
    }
    
    // 0 from 1  (1 -> CCW)
    i_table[1][0][1] = vec2(+2,+0);
    i_table[1][0][2] = vec2(-1,+0);
    i_table[1][0][3] = vec2(+2,+1);
    i_table[1][0][4] = vec2(-1,-2);
    
    // 1 from 0  (0 -> CW)
    i_table[0][1][1] = vec2(-2,+0);
    i_table[0][1][2] = vec2(+1,+0);
    i_table[0][1][3] = vec2(+1,+2);
    i_table[0][1][4] = vec2(-2,-1);
    
    
    // 1 from 2  (2 -> CCW)
    i_table[2][0][1] = vec2(-2,+0);
    i_table[2][0][2] = vec2(+1,+0);
    i_table[2][0][3] = vec2(-2,+1);
    i_table[2][0][4] = vec2(+1,-1);
    
    // 2 from 1  (1 -> CW)
    i_table[1][1][1] = vec2(-1,+0);
    i_table[1][1][2] = vec2(+2,+0);
    i_table[1][1][3] = vec2(-1,+2);
    i_table[1][1][4] = vec2(+2,-1);
    
    
    // 2 from 3  (3 -> CCW)
    i_table[3][0][1] = vec2(+1,+0);
    i_table[3][0][2] = vec2(-2,+0);
    i_table[3][0][3] = vec2(+1,+2);
    i_table[3][0][4] = vec2(-2,-1);
    
    // 3 from 2  (2 -> CW)
    i_table[2][1][1] = vec2(+2,+0);
    i_table[2][1][2] = vec2(-1,+0);
    i_table[2][1][3] = vec2(+2,+1);
    i_table[2][1][4] = vec2(-1,-1);
    
    
    // 3 from 0  (0 -> CCW)
    i_table[0][0][1] = vec2(+2,+0);
    i_table[0][0][2] = vec2(-1,+0);
    i_table[0][0][3] = vec2(-1,+2);
    i_table[0][0][4] = vec2(+2,-1);
    
    // 0 from 3  (3 -> CW)
    i_table[3][1][1] = vec2(-2,+0);
    i_table[3][1][2] = vec2(+1,+0);
    i_table[3][1][3] = vec2(-2,+1);
    i_table[3][1][4] = vec2(+1,-2);
    
}

// dir = 1 for CW, 0 for CCW
vec2 KickTable::getOffset(int fromState, int dir, int kicks)
{
//    if ( !((fromState>=0 && fromState < 4) &&
//           (dir == 0 || dir == 1) &&
//           (kicks >= 0 && kicks < 5)) )
//    {
//        std::cerr << "invalid kick args:"
//                  << " from=" << fromState
//                  << " dir=" << dir
//                  << " kicks=" << kicks << std::endl;
//        return vec2(0,0);
//    }
    
    return main_table[fromState][dir][kicks];
}

vec2 KickTable::getIOffset(int fromState, int dir, int kicks)
{
    return i_table[fromState][dir][kicks];
}


