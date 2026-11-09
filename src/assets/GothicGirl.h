#pragma once
#include "animation/Animation2D.h"

struct GothicGirlAsset
{
    Mesh mesh;
    std::vector<VertexInfluence> influences;
};

GothicGirlAsset makeGothicGirl();
Pose2D makeGothicGirlPose(float timeSeconds);
