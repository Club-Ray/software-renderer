#pragma once

#include <cstdint>
#include <vector>

class Renderer
{
public:
    Renderer(int width, int height);

    void clear(std::uint8_t red, std::uint8_t green, std::uint8_t blue);

    [[nodiscard]] int width() const;
    [[nodiscard]] int height() const;
    [[nodiscard]] const std::uint32_t* pixels() const;
	[[nodiscard]] int index(int x, int y) const;

    // 2 functions
    // 1. draw pixel
	void DrawPixel(int x, int y, std::uint32_t color);
	// 2. draw line (bresenham's line algorithm)
	void DrawLine(int x0, int y0, int x1, int y1, std::uint32_t color);

private:
    int width_;
    int height_;
    std::vector<std::uint32_t> pixels_;
};
