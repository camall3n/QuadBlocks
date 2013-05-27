//
//  mesh.cpp
//  Tetris
//
//  Created by Cam Allen on 5/4/13.
//  Copyright (c) 2013 Cam Allen. All rights reserved.
//

#include "mesh.h"

#include <cstdio>
#include <cstdlib>
#include <boost/foreach.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/tokenizer.hpp>
#include <string>

Mesh::Mesh()
{
}

Mesh::~Mesh()
{
}

// Load mesh from the specified XML file
void Mesh::load(const std::string filename)
{
    using boost::property_tree::ptree;
    using boost::char_separator;
    using boost::tokenizer;
    
    // Create an empty property tree object
    ptree pt;
    
    // Load the XML file into the property tree. If reading fails
    // (cannot open file, parse error), an exception is thrown.
    read_xml(filename, pt);

    // Get the name of the mesh
    name = pt.get<std::string>("mesh.name");
    
    // Set the list of separator characters
    char_separator<char> sep(", \n");
    
    // Get the vertices for the mesh
    this->vertices.clear();
    std::string verticesStr = pt.get<std::string>("mesh.vertices");
    tokenizer< char_separator<char> > vertices(verticesStr, sep);
    BOOST_FOREACH(const std::string vertex_coord, vertices) {
        float value = atof(vertex_coord.c_str());
        this->vertices.push_back(value);
    }
    
    // Get the indices for the mesh
    this->indices.clear();
    std::string indicesStr = pt.get<std::string>("mesh.indices");
    tokenizer< char_separator<char> > indices(indicesStr, sep);
    BOOST_FOREACH(const std::string index, indices) {
        int value = atoi(index.c_str());
        this->indices.push_back(value);
    }
}

// Save mesh to the specified XML file
void Mesh::save(const std::string filename)
{
    using boost::property_tree::ptree;
    using boost::property_tree::xml_writer_settings;
    using boost::char_separator;
    using boost::tokenizer;
    
    // Create an empty property tree object
    ptree pt;
    
    // Set the name of the mesh
    pt.put("mesh.name", name);
    
    // Set the list of separator characters
    char_separator<char> sep(", \n");
    
    // Set the vertices for the mesh
    // NOTE: The number of spaces before each entry is based on the
    //       property's seniority in the tree, which is always 2.
    std::string verticesStr = "\n    ";
    int i=1;
    BOOST_FOREACH(const float vertex_coord, this->vertices) {
        char valueStr[15];
        std::sprintf(valueStr, "% g", vertex_coord);
        
        if (i%3==1) {
            verticesStr += "    ";
        }
        verticesStr += valueStr;
        verticesStr += ", ";
        if (i%3==0) {
            verticesStr += "\n    ";
        }
        i++;
    }
    pt.put("mesh.vertices", verticesStr);
    
    
    // Set the indices for the mesh
    // NOTE: The number of spaces before each entry is based on the
    //       property's seniority in the tree, which is always 2.
    std::string indicesStr = "\n    ";
    i=1;
    BOOST_FOREACH(const int index, this->indices) {
        char valueStr[15];
        std::sprintf(valueStr, "%d", index);
        
        if (i%3==1) {
            indicesStr += "    ";
        }
        indicesStr += valueStr;
        indicesStr += ", ";
        if (i%3==0) {
            indicesStr += "\n    ";
        }
        i++;
    }
    pt.put("mesh.indices", indicesStr);
    
    // Write property tree to the XML file
    xml_writer_settings<char> settings(' ', 4);// Indent with 4 spaces
    write_xml(filename, pt, std::locale(), settings);
}

size_t Mesh::nVertices()
{
    return vertices.size();
}

size_t Mesh::nIndices()
{
    return indices.size();
}

void Mesh::fillVertexArray(float* vertexArray)
{
    int i=0;
    BOOST_FOREACH(float vertex_coord, vertices) {
        vertexArray[i] = vertex_coord;
        i++;
    }
}

void Mesh::fillIndexArray(GLshort* indexArray)
{
    int i=0;
    BOOST_FOREACH(int index, indices) {
        indexArray[i] = index;
        i++;
    }
}
