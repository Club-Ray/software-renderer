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

private:
    int width_;
    int height_;
    std::vector<std::uint32_t> pixels_;
};
