#include "assets/TextureLoader.h"

#include <SDL3/SDL.h>

#include <cassert>
#include <climits>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <limits>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#define STBI_NO_SIMD
#define STBI_NO_STDIO
#define STBI_NO_THREAD_LOCALS
#define STBI_FAILURE_USERMSG
#define STBI_ONLY_JPEG
#define STBI_NO_GIF
#define STBI_NO_HDR
#define STBI_NO_LINEAR
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Texture loadTexture(const std::filesystem::path& path)
{
    int width = 0;
    int height = 0;
    int sourceChannelCount = 0;
    constexpr int requestedChannelCount = STBI_rgb_alpha;

    std::size_t encodedByteCount = 0;
    void* encodedBytes = SDL_LoadFile(path.string().c_str(), &encodedByteCount);
    if (encodedBytes == nullptr)
    {
        throw std::runtime_error(
            "Could not read texture '" + path.string() + "': " + SDL_GetError());
    }

    if (encodedByteCount > static_cast<std::size_t>(std::numeric_limits<int>::max()))
    {
        SDL_free(encodedBytes);
        throw std::runtime_error("Texture file is too large for stb_image");
    }

    stbi_uc* decodedPixels = stbi_load_from_memory(
        static_cast<const stbi_uc*>(encodedBytes),
        static_cast<int>(encodedByteCount),
        &width,
        &height,
        &sourceChannelCount,
        requestedChannelCount);
    SDL_free(encodedBytes);

    if (decodedPixels == nullptr)
    {
        // SDL's vendored stb_image reports decoder failures through SDL_SetError.
        const char* reason = SDL_GetError();
        throw std::runtime_error(
            "Could not load texture '" + path.string() + "': " +
            (reason != nullptr ? reason : "unknown stb_image error"));
    }

    const auto pixelCount =
        static_cast<std::size_t>(width) * static_cast<std::size_t>(height);
    std::vector<std::uint32_t> pixels(pixelCount);

    // stb_image returns RGBA bytes. The CPU framebuffer stores colors as
    // 0xAARRGGBB, matching the renderer's existing color convention.
    for (std::size_t pixelIndex = 0; pixelIndex < pixelCount; ++pixelIndex)
    {
        const std::size_t byteIndex = pixelIndex * requestedChannelCount;
        const auto red = static_cast<std::uint32_t>(decodedPixels[byteIndex]);
        const auto green = static_cast<std::uint32_t>(decodedPixels[byteIndex + 1]);
        const auto blue = static_cast<std::uint32_t>(decodedPixels[byteIndex + 2]);
        const auto alpha = static_cast<std::uint32_t>(decodedPixels[byteIndex + 3]);

        pixels[pixelIndex] =
            (alpha << 24u) | (red << 16u) | (green << 8u) | blue;
    }

    stbi_image_free(decodedPixels);
    return Texture(width, height, std::move(pixels));
}
