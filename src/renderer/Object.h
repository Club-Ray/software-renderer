#pragma once
#include "renderer/Mesh.h"

struct Object
{
    Mesh mesh_;
    // Empty for static objects. Animation writes here, leaving mesh_ intact.
    std::vector<Vertex> posedVertices_;
    Vec2 scale_{1.0f, 1.0f};
    float rotation_ = 0.0f; // Radians, around the local origin.
    Vec2 position_;
};
