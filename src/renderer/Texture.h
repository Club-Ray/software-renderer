#pragma once

#include <cstdint>
#include <vector>

class Texture
{
public:
    Texture(int width, int height, std::vector<std::uint32_t> pixels);

    [[nodiscard]] int width() const;
    [[nodiscard]] int height() const;

    // Nearest-neighbor sampling. UV (0, 0) is the top-left texel and
    // UV (1, 1) is the bottom-right texel. Values outside that range clamp.
    [[nodiscard]] std::uint32_t sampleNearest(float u, float v) const;

private:
    int width_;
    int height_;
    std::vector<std::uint32_t> pixels_;
};
