#pragma once

#include "Vertex.h"

class Face
{
public:
    Face(int mtl, Vertex a, vTexture c, Vertex b, bool texture = true);
    Face(int mtl, Vertex a, Vertex b, bool texture = false);

public:
    int material;
    bool hasTexture;
    Vertex v, n;     // First, Second, and Third Vertices of Face
    vTexture t;
    // Normal x, y, z;     // Normal
};