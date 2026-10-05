#include "assets/GothicGirlMesh.h"

Mesh makeGothicGirlMesh()
{
    // Static grid from the stash: positions and UVs share normalized,
    // top-left-origin coordinates. No pose or bone transforms are applied.
    constexpr int columns = 32;
    constexpr int rows = 48;
    Mesh mesh;
    mesh.triangles_.reserve(columns * rows * 2);

    for (int row = 0; row < rows; ++row)
    {
        for (int column = 0; column < columns; ++column)
        {
            const float left = static_cast<float>(column) / columns;
            const float right = static_cast<float>(column + 1) / columns;
            const float top = static_cast<float>(row) / rows;
            const float bottom = static_cast<float>(row + 1) / rows;
            const Vertex topLeft{{left, top}, {left, top}, 0xFFFFFFFFu};
            const Vertex topRight{{right, top}, {right, top}, 0xFFFFFFFFu};
            const Vertex bottomLeft{{left, bottom}, {left, bottom}, 0xFFFFFFFFu};
            const Vertex bottomRight{{right, bottom}, {right, bottom}, 0xFFFFFFFFu};
            mesh.triangles_.push_back(Triangle{{topLeft, topRight, bottomLeft}});
            mesh.triangles_.push_back(Triangle{{topRight, bottomRight, bottomLeft}});
        }
    }
    return mesh;
}
