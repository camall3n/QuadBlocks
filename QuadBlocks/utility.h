//
//  utility.h
//  QuadBlocks
//
//  Created by Cam Allen on 5/8/13.
//  Copyright (c) 2013 Cam Allen. All rights reserved.
//

#ifndef __QuadBlocks__utility__
#define __QuadBlocks__utility__

#include <cstdlib>
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

int randint(int a, int b);// random integer in the range [a, b]

#endif /* defined(__QuadBlocks__utility__) */
