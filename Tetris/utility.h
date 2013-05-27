//
//  utility.h
//  Tetris
//
//  Created by Cam Allen on 5/8/13.
//  Copyright (c) 2013 Cam Allen. All rights reserved.
//

#ifndef __Tetris__utility__
#define __Tetris__utility__

#include <iostream>
#include <string>

#include <GL/glew.h>
#include <GL/glfw.h>
#include <glm/glm.hpp>
using std::cout;
using std::endl;

void checkError();
void checkError(std::string s);

GLuint LoadProgram(const char * vertex_file_path, const char * fragment_file_path);

std::ostream &operator<< (std::ostream &out, const glm::vec2 &vec);
std::ostream &operator<< (std::ostream &out, const glm::vec3 &vec);
std::ostream &operator<< (std::ostream &out, const glm::vec4 &vec);

std::ostream &operator<< (std::ostream &out, const glm::mat2 &vec);
std::ostream &operator<< (std::ostream &out, const glm::mat3 &vec);
std::ostream &operator<< (std::ostream &out, const glm::mat4 &vec);


#endif /* defined(__Tetris__utility__) */
