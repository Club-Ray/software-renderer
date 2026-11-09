#include "animation/Animation2D.h"
#include <stdexcept>

Vec2 skinVertex(Vec2 bindPosition, const VertexInfluence& influence, const Pose2D& pose)
{
    Vec2 local{};
    for (const BoneWeight& connection : influence.bones)
    {
        const float weight = connection.weight;
        if (weight == 0.0f) continue;

        Vec2 transformed = bindPosition;
        const int boneIndex = connection.boneIndex;
        if (boneIndex >= 0)
        {
            const Bone2D& bone = pose.bones.at(static_cast<std::size_t>(boneIndex));
            transformed = rotateAround(bindPosition, bone.bindPivot, bone.angleRadians);
        }
        // Each bone transforms the same bind position independently.
        // Blend the results; do not rotate one bone's result by the next bone.
        local.x += weight * transformed.x;
        local.y += weight * transformed.y;
    }
    // Local deformation first, whole-body rotation second.
    return rotateAround(local, pose.rootPivot, pose.rootAngleRadians);
}

void skinMesh(const Mesh& mesh, const std::vector<VertexInfluence>& influences,
              const Pose2D& pose, std::vector<Vertex>& posedVertices)
{
    if (influences.size() != mesh.vertices.size())
        throw std::invalid_argument("Each mesh vertex needs one animation influence");

    posedVertices.resize(mesh.vertices.size());
    for (std::size_t i = 0; i < mesh.vertices.size(); ++i)
    {
        posedVertices[i] = mesh.vertices[i];
        posedVertices[i].position = skinVertex(mesh.vertices[i].position, influences[i], pose);
    }
}
