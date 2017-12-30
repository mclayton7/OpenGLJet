#include "Face.h"

Face::Face(int mtl, Vertex a, vTexture c, Vertex b, bool texture)
	: material(mtl)
	, v(a)
	, t(c)
	, n(b)
	, hasTexture(texture)
{
}

Face::Face(int mtl, Vertex a, Vertex b, bool texture)
	: material(mtl)
	, v(a)
	, n(b)
	, hasTexture(texture)
{
}
