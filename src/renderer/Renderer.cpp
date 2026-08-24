#include "renderer/Renderer.h"

#include <algorithm>
#include <stdexcept>

namespace
{
std::size_t pixelCount(int width, int height)
{
    if (width <= 0 || height <= 0)
    {
        throw std::invalid_argument("Renderer dimensions must be positive");
    }

    return static_cast<std::size_t>(width) * static_cast<std::size_t>(height);
}
}

Renderer::Renderer(int width, int height)
    : width_(width),
      height_(height),
      pixels_(pixelCount(width, height))
{
}

void Renderer::clear(std::uint8_t red, std::uint8_t green, std::uint8_t blue)
{
    const std::uint32_t color =
        0xFF000000u |
        (static_cast<std::uint32_t>(red) << 16u) |
        (static_cast<std::uint32_t>(green) << 8u) |
        static_cast<std::uint32_t>(blue);

    std::fill(pixels_.begin(), pixels_.end(), color);
}

int Renderer::width() const
{
    return width_;
}

int Renderer::height() const
{
    return height_;
}

const std::uint32_t* Renderer::pixels() const
{
    return pixels_.data();
}
