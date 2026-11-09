#include "assets/GothicGirl.h"

#include <algorithm>
#include <cmath>

namespace
{
enum GothicBone { Head, LeftHair, RightHair, LeftArm, RightArm, Skirt };

float smoothstep(float low, float high, float value)
{
    const float t = std::clamp((value - low) / (high - low), 0.0f, 1.0f);
    return t * t * (3.0f - 2.0f * t);
}

}

GothicGirlAsset makeGothicGirl()
{
    // A regular grid keeps the triangles easy to inspect. UV and bind position
    // coincide, so the unposed mesh reproduces the source PNG exactly.
    constexpr std::uint32_t columns = 32;
    constexpr std::uint32_t rows = 48;
    GothicGirlAsset asset;
    Mesh& mesh = asset.mesh;
    mesh.vertices.reserve((columns + 1) * (rows + 1));
    asset.influences.reserve((columns + 1) * (rows + 1));
    mesh.triangles.reserve(columns * rows * 2);

    for (std::uint32_t row = 0; row <= rows; ++row)
    {
        const float y = static_cast<float>(row) / static_cast<float>(rows);
        for (std::uint32_t column = 0; column <= columns; ++column)
        {
            const float x = static_cast<float>(column) / static_cast<float>(columns);
            VertexInfluence vertex;

            // Smooth masks blend each moving part into the stationary body.
            // The transparent PNG tolerates the rectangular grid around it.
            vertex.bones = {
                {Head,
                    smoothstep(0.33f, 0.40f, x) *
                    (1.0f - smoothstep(0.60f, 0.67f, x)) *
                    (1.0f - smoothstep(0.15f, 0.25f, y)) * 0.85f},
                {LeftHair,
                    (1.0f - smoothstep(0.32f, 0.48f, x)) *
                    smoothstep(0.05f, 0.16f, y) *
                    (1.0f - smoothstep(0.42f, 0.59f, y)) * 0.95f},
                {RightHair,
                    smoothstep(0.52f, 0.68f, x) *
                    smoothstep(0.05f, 0.16f, y) *
                    (1.0f - smoothstep(0.42f, 0.59f, y)) * 0.95f},
                {LeftArm,
                    (1.0f - smoothstep(0.29f, 0.42f, x)) *
                    smoothstep(0.19f, 0.32f, y) *
                    (1.0f - smoothstep(0.48f, 0.56f, y)) * 0.85f},
                {RightArm,
                    smoothstep(0.58f, 0.71f, x) *
                    smoothstep(0.19f, 0.32f, y) *
                    (1.0f - smoothstep(0.48f, 0.56f, y)) * 0.85f},
                {Skirt,
                    smoothstep(0.32f, 0.49f, y) *
                    (1.0f - smoothstep(0.70f, 0.83f, y)) * 0.55f},
            };
            float total = 0.0f;
            for (const BoneWeight& connection : vertex.bones) total += connection.weight;
            // Preserve stationary-body influence where local masks are weak.
            // -1 is the root's unchanged local position, before whole-body sway.
            vertex.bones.push_back({-1, std::max(0.0f, 1.0f - total)});
            total = std::max(1.0f, total);
            for (BoneWeight& connection : vertex.bones) connection.weight /= total;

            mesh.vertices.push_back({{x, y}, {x, y}, 0xFFFFFFFFu});
            asset.influences.push_back(vertex);
        }
    }

    for (std::uint32_t row = 0; row < rows; ++row)
    {
        for (std::uint32_t column = 0; column < columns; ++column)
        {
            const std::uint32_t topLeft = row * (columns + 1) + column;
            const std::uint32_t topRight = topLeft + 1;
            const std::uint32_t bottomLeft = topLeft + columns + 1;
            const std::uint32_t bottomRight = bottomLeft + 1;
            mesh.triangles.push_back({topLeft, topRight, bottomLeft});
            mesh.triangles.push_back({topRight, bottomRight, bottomLeft});
        }
    }

    return asset;
}

Pose2D makeGothicGirlPose(float timeSeconds)
{
    constexpr float degrees = 0.01745329252f;
    Pose2D pose;
    pose.rootPivot = {0.5f, 0.39f};
    pose.rootAngleRadians = 1.3f * degrees * std::sin(timeSeconds * 1.1f);
    pose.bones = {
        {{0.50f, 0.19f}, 1.6f * degrees * std::sin(timeSeconds * 1.0f + 0.5f)},
        {{0.37f, 0.10f}, 3.5f * degrees * std::sin(timeSeconds * 1.2f + 0.8f)},
        {{0.63f, 0.10f}, -3.5f * degrees * std::sin(timeSeconds * 1.2f + 0.8f)},
        {{0.34f, 0.26f}, 2.0f * degrees * std::sin(timeSeconds * 1.4f)},
        {{0.66f, 0.26f}, -2.0f * degrees * std::sin(timeSeconds * 1.4f)},
        {{0.50f, 0.37f}, 1.8f * degrees * std::sin(timeSeconds * 1.1f + 1.0f)}
    };
    return pose;
}
