#pragma once
#include "math/Math2D.h"
#include <array>
#include <cstdint>
#include <vector>

struct UV
{
    float u = 0.0f;
    float v = 0.0f;
};

struct Vertex
{
    Vec2 position;
    UV uv;
    std::uint32_t color = 0xFFFFFFFFu;
};

using TriangleIndices = std::array<std::uint32_t, 3>;

struct Mesh
{
    // Original local positions remain unchanged, including during animation.
    std::vector<Vertex> vertices;
    std::vector<TriangleIndices> triangles;
};
