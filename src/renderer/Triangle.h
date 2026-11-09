#pragma once
#include "renderer/Mesh.h"

// Three assembled screen-space vertices, ready for rasterization.
struct Triangle
{
    Vertex v[3];
};
