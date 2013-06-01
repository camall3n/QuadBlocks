//
//  utility.cpp
//  Tetris
//
//  Created by Cam Allen on 5/8/13.
//  Copyright (c) 2013 Cam Allen. All rights reserved.
//

#include "utility.h"

#include <cstdio>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

void checkError()
{
    GLenum error;
    while ( (error = glGetError()) != GL_NO_ERROR ) {
        printf("%s\n", gluErrorString(error));
    }
}

void checkError(std::string s)
{
    GLenum error;
    while ( (error = glGetError()) != GL_NO_ERROR ) {
        printf("%s %s\n", gluErrorString(error), s.c_str());
    }
}

GLuint LoadProgram(const char * vertex_file_path, const char * fragment_file_path){
    
	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
    
	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
	if(VertexShaderStream.is_open()){
		std::string Line = "";
		while(getline(VertexShaderStream, Line))
			VertexShaderCode += "\n" + Line;
		VertexShaderStream.close();
	}else{
		printf("Failed to open '%s'.\n", vertex_file_path);
		return 0;
	}
    
	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
	if(FragmentShaderStream.is_open()){
		std::string Line = "";
		while(getline(FragmentShaderStream, Line))
			FragmentShaderCode += "\n" + Line;
		FragmentShaderStream.close();
	}
    
    
    
	GLint Result = GL_FALSE;
	int InfoLogLength;
    
    
    
	// Compile Vertex Shader
	char const * VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
	glCompileShader(VertexShaderID);
    
	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
        printf("Error compiling vertex shader : %s\n", vertex_file_path);
		std::vector<char> VertexShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}
    
    
    
	// Compile Fragment Shader
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
	glCompileShader(FragmentShaderID);
    
	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
        printf("Error compiling fragment shader : %s\n", fragment_file_path);
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}
    
    
    
	// Link the program
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);
    
	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
        printf("Error linking program\n");
		std::vector<char> ProgramErrorMessage(InfoLogLength+1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}
    
	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);
    
	return ProgramID;
}

static const int w=3;
std::ostream &operator<< (std::ostream &out, const glm::vec2 &vec) {
    out << "{"
        << std::setw(w) << vec.x << " "
        << std::setw(w) << vec.y
        << "}";
    return out;
}
std::ostream &operator<< (std::ostream &out, const glm::vec3 &vec) {
    out << "{"
        << std::setw(w) << vec.x << " "
        << std::setw(w) << vec.y << " "
        << std::setw(w) << vec.z
        << "}";
    return out;
}
std::ostream &operator<< (std::ostream &out, const glm::vec4 &vec) {
    out << "{"
        << std::setw(w) << vec.x << " "
        << std::setw(w) << vec.y << " "
        << std::setw(w) << vec.z << " "
        << std::setw(w) << vec.w
        << "}";
    return out;
}

std::ostream &operator<< (std::ostream &out, const glm::mat2 &mat) {
    out << "{" << mat[0] << endl
        << " " << mat[1] << "}";
    return out;
}
std::ostream &operator<< (std::ostream &out, const glm::mat3 &mat) {
    out << "{" << mat[0] << endl
        << " " << mat[1] << endl
        << " " << mat[2] << "}";
    return out;
}
std::ostream &operator<< (std::ostream &out, const glm::mat4 &mat) {
    out << "{" << mat[0] << endl
        << " " << mat[1] << endl
        << " " << mat[2] << endl
        << " " << mat[3] << "}";
    return out;
}

int randint(int a, int b)
{
    return ( rand() % (b-a) ) + a;
}