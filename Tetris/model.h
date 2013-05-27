//
//  model.h
//  Tetris
//
//  Created by Cam Allen on 5/12/13.
//  Copyright (c) 2013 Cam Allen. All rights reserved.
//

#ifndef __Tetris__model__
#define __Tetris__model__

#include <GL/glew.h>

class Model
{
public:
    friend class GraphicsManager;
    
private:
    GLuint _baseProgram;
};

#endif /* defined(__Tetris__model__) */
