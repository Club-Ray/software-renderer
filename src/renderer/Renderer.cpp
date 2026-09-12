#include "renderer/Renderer.h"

#include "assets/Texture.h"

#include <algorithm>
#include <cstdlib>
#include <limits>
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

void Renderer::fill_triangle(
    int x0, int y0, std::uint32_t color0,
    int x1, int y1, std::uint32_t color1,
    int x2, int y2, std::uint32_t color2)
{
    const int minX = std::max(0, std::min({x0, x1, x2}));
    const int maxX = std::min(width_ - 1, std::max({x0, x1, x2}));
    const int minY = std::max(0, std::min({y0, y1, y2}));
    const int maxY = std::min(height_ - 1, std::max({y0, y1, y2}));

    const double denominator =
        static_cast<double>(y1 - y2) * (x0 - x2) +
        static_cast<double>(x2 - x1) * (y0 - y2);

    if (denominator == 0.0)
    {
        return;
    }

    for (int y = minY; y <= maxY; ++y)
    {
        for (int x = minX; x <= maxX; ++x)
        {
            const double weight0 =
                (static_cast<double>(y1 - y2) * (x - x2) +
                 static_cast<double>(x2 - x1) * (y - y2)) /
                denominator;
            const double weight1 =
                (static_cast<double>(y2 - y0) * (x - x2) +
                 static_cast<double>(x0 - x2) * (y - y2)) /
                denominator;
            const double weight2 = 1.0 - weight0 - weight1;

            constexpr double epsilon = std::numeric_limits<double>::epsilon();

            if (weight0 < -epsilon ||
                weight1 < -epsilon ||
                weight2 < -epsilon)
            {
                continue;
            }

            const std::uint32_t alpha = static_cast<std::uint32_t>(
                weight0 * ((color0 >> 24u) & 0xFFu) +
                weight1 * ((color1 >> 24u) & 0xFFu) +
                weight2 * ((color2 >> 24u) & 0xFFu));
            const std::uint32_t red = static_cast<std::uint32_t>(
                weight0 * ((color0 >> 16u) & 0xFFu) +
                weight1 * ((color1 >> 16u) & 0xFFu) +
                weight2 * ((color2 >> 16u) & 0xFFu));
            const std::uint32_t green = static_cast<std::uint32_t>(
                weight0 * ((color0 >> 8u) & 0xFFu) +
                weight1 * ((color1 >> 8u) & 0xFFu) +
                weight2 * ((color2 >> 8u) & 0xFFu));
            const std::uint32_t blue = static_cast<std::uint32_t>(
                weight0 * (color0 & 0xFFu) +
                weight1 * (color1 & 0xFFu) +
                weight2 * (color2 & 0xFFu));

            DrawPixel(
                x,
                y,
                (alpha << 24u) | (red << 16u) | (green << 8u) | blue);
        }
    }
}

void Renderer::fill_triangle(
    const Texture& texture,
    int x0, int y0, UV uv0,
    int x1, int y1, UV uv1,
    int x2, int y2, UV uv2)
{
    const int minX = std::max(0, std::min({ x0, x1, x2 }));
    const int maxX = std::min(width_ - 1, std::max({ x0, x1, x2 }));
    const int minY = std::max(0, std::min({ y0, y1, y2 }));
    const int maxY = std::min(height_ - 1, std::max({ y0, y1, y2 }));

    const double denominator =
        static_cast<double>(y1 - y2) * (x0 - x2) +
        static_cast<double>(x2 - x1) * (y0 - y2);

    if (denominator == 0.0)
    {
        return;
    }

    for (int y = minY; y <= maxY; ++y)
    {
        for (int x = minX; x <= maxX; ++x)
        {
            const double weight0 =
                (static_cast<double>(y1 - y2) * (x - x2) +
                    static_cast<double>(x2 - x1) * (y - y2)) /
                denominator;
            const double weight1 =
                (static_cast<double>(y2 - y0) * (x - x2) +
                    static_cast<double>(x0 - x2) * (y - y2)) /
                denominator;
            const double weight2 = 1.0 - weight0 - weight1;

            constexpr double epsilon = std::numeric_limits<double>::epsilon();

            if (weight0 < -epsilon ||
                weight1 < -epsilon ||
                weight2 < -epsilon)
            {
                continue;
            }

            // Barycentric weights interpolate the vertex UVs at this pixel.
            const float u = static_cast<float>(
                weight0 * uv0.u + weight1 * uv1.u + weight2 * uv2.u);
            const float v = static_cast<float>(
                weight0 * uv0.v + weight1 * uv1.v + weight2 * uv2.v);

            DrawPixel(x, y, texture.sampleNearest(u, v));
        }
    }
}
