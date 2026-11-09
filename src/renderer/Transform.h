#pragma once

#include "math/Math3D.h"

struct Transform
{
    Vec3 scale{1.0f, 1.0f, 1.0f};
    Vec3 rotation{1.0f, 1.0f, 1.0f}; // Radians, around the local origin.
    Vec3 position{1.0f, 1.0f, 1.0f};
};
