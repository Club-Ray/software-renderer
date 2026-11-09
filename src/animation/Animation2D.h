#pragma once
#include "renderer/Mesh.h"
#include <vector>

struct BoneWeight
{
    int boneIndex = -1; // -1 means the unchanged local/body position.
    float weight = 0.0f;
};

struct VertexInfluence
{
    // Each connection keeps its bone and weight together. No fixed limit.
    // Weights are nonnegative and sum to one.
    std::vector<BoneWeight> bones{{-1, 1.0f}};
};

struct Bone2D
{
    Vec2 bindPivot;
    float angleRadians = 0.0f;
};

struct Pose2D
{
    Vec2 rootPivot;
    float rootAngleRadians = 0.0f;
    std::vector<Bone2D> bones;
};

Vec2 skinVertex(Vec2 bindPosition, const VertexInfluence& influence, const Pose2D& pose);
void skinMesh(const Mesh& mesh, const std::vector<VertexInfluence>& influences,
              const Pose2D& pose, std::vector<Vertex>& posedVertices);
