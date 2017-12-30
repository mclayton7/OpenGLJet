#pragma once

#include "OpenGLSupport.h"

class Vertex
{
public:
    Vertex(GLfloat a, GLfloat b, GLfloat c);
    Vertex();
public:
	// X, Y, and Z Vertices
    GLfloat x;
    GLfloat y;
    GLfloat z;
};

typedef Vertex Normal;
typedef Vertex vTexture;
