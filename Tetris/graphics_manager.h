//
//  graphics_manager.h
//  Tetris
//
//  Created by Cam Allen on 5/11/13.
//  Copyright (c) 2013 Cam Allen. All rights reserved.
//

#ifndef __Tetris__graphics_manager__
#define __Tetris__graphics_manager__

#include <boost/noncopyable.hpp>
#include <GL/glew.h>
#include "model.h"

class GraphicsManager : boost::noncopyable
{
public:
    GraphicsManager();
    ~GraphicsManager();
    
    void onResize(int x, int y);
    GLuint basicProgram;
    
private:
    void RegisterModel(Model m);
    
};


#endif /* defined(__Tetris__graphics_manager__) */
