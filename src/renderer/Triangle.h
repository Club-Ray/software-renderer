#pragma once

struct Vec2 {
    Vec2(): x(0.0f), y(0.0f) {}
	Vec2(float a, float b) : x(a), y(b) {}
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
