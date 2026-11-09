#pragma once
#include "renderer/Triangle.h"
#include "renderer/Object.h"
#include <cstdint>
#include <vector>

class Texture;

class Renderer
{
public:
    Renderer(int width, int height);
    void clear(std::uint8_t red, std::uint8_t green, std::uint8_t blue);
    int width() const;
    int height() const;
    const std::uint32_t* pixels() const;
    int index(int x, int y) const;
    void DrawPixel(int x, int y, std::uint32_t color);
    void DrawLine(int x0, int y0, int x1, int y1, std::uint32_t color);
    void fill_triangle(const Triangle& triangle);
    void fill_triangle(const Texture& texture, const Triangle& triangle);
    void draw(const Object& object, const Texture* texture = nullptr, bool wireframe = false);

private:
    // One coverage loop for both vertex colors and textures.
    void rasterize(const Triangle& triangle, const Texture* texture);
    int width_;
    int height_;
    std::vector<std::uint32_t> pixels_;
    std::vector<Vertex> screenVertices_; // Reused across draws.
};
