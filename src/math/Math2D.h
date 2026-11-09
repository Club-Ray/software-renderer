#pragma once

#include <glm/vec2.hpp>

#include <cmath>

using Vec2 = glm::vec2;

// Positive angles rotate clockwise in our screen coordinates (y points down).
inline Vec2 rotate(Vec2 point, float cosine, float sine)
{
    return {point.x * cosine - point.y * sine,
            point.x * sine + point.y * cosine};
}

inline Vec2 rotateAround(Vec2 point, Vec2 pivot, float radians)
{
    const Vec2 local{point.x - pivot.x, point.y - pivot.y};
    const Vec2 rotated = rotate(local, std::cos(radians), std::sin(radians));
    return {pivot.x + rotated.x, pivot.y + rotated.y};
}
