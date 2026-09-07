#include "renderer/Renderer.h"

#include <algorithm>
#include <cstdlib>
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

int Renderer::index(int x, int y) const
{
    return x + y * width_;
}

void Renderer::DrawPixel(int x, int y, std::uint32_t color)
{
    pixels_[index(x, y)] = color;
}

void Renderer::DrawLine(int x0, int y0, int x1, int y1, std::uint32_t color)
{
    const std::int64_t dx = std::abs(static_cast<std::int64_t>(x1) - x0);
    const std::int64_t dy = -std::abs(static_cast<std::int64_t>(y1) - y0);
    const int stepX = x0 < x1 ? 1 : -1;
    const int stepY = y0 < y1 ? 1 : -1;
    std::int64_t error = dx + dy;

    // Bresenham's error selects an x step, a y step, or both for any slope.
    while (true)
    {
        if (x0 >= 0 && x0 < width_ && y0 >= 0 && y0 < height_)
        {
            DrawPixel(x0, y0, color);
        }

        // Include both endpoints; a zero-length line draws one pixel.
        if (x0 == x1 && y0 == y1)
        {
            break;
        }

        const std::int64_t twiceError = 2 * error;
        if (twiceError >= dy)
        {
            error += dy;
            x0 += stepX;
        }
        if (twiceError <= dx)
        {
            error += dx;
            y0 += stepY;
        }
    }
}
