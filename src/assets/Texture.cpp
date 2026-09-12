#include "assets/Texture.h"

#include <algorithm>
#include <cstddef>
#include <stdexcept>
#include <utility>

Texture::Texture(int width, int height, std::vector<std::uint32_t> pixels)
    : width_(width),
      height_(height),
      pixels_(std::move(pixels))
{
    if (width_ <= 0 || height_ <= 0)
    {
        throw std::invalid_argument("Texture dimensions must be positive");
    }

    const auto expectedPixelCount =
        static_cast<std::size_t>(width_) * static_cast<std::size_t>(height_);
    if (pixels_.size() != expectedPixelCount)
    {
        throw std::invalid_argument("Texture pixel count does not match its dimensions");
    }
}

int Texture::width() const
{
    return width_;
}

int Texture::height() const
{
    return height_;
}

std::uint32_t Texture::sampleNearest(float u, float v) const
{
    const float clampedU = std::clamp(u, 0.0f, 1.0f);
    const float clampedV = std::clamp(v, 0.0f, 1.0f);

    const int x = static_cast<int>(clampedU * static_cast<float>(width_ - 1) + 0.5f);
    const int y = static_cast<int>(clampedV * static_cast<float>(height_ - 1) + 0.5f);
    const auto pixelIndex = static_cast<std::size_t>(x + y * width_);

    return pixels_[pixelIndex];
}
