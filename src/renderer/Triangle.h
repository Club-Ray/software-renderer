#pragma once

struct Vec2 {
    float x;
    float y;
};

struct UV {
    float u;
    float v;
};

struct Vertex {
    Vec2 position;
    UV uv;
    std::uint32_t color;
};

struct Triangle {
    Vertex v[3];
};
