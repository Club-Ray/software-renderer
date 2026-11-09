#pragma once

#include "math/Math2D.h"
#include "math/Math3D.h"

#include <array>
#include <cstdint>
#include <vector>

struct Vertex
{
    Vec3 position{0.0f, 0.0f, 0.0f};
    Vec2 uv{0.0f, 0.0f};
    std::uint32_t color = 0xFFFFFFFFu;
};

struct Triangle
{
    Vertex vertices[3];
};

using TriangleIndices = std::array<std::uint32_t, 3>;

struct Mesh
{
    std::vector<Vertex> vertices;
    std::vector<TriangleIndices> triangles;
};
