#include "platform/sdl/SdlPlatform.h"

#include "renderer/Renderer.h"

#include <SDL3/SDL.h>

#include <stdexcept>
#include <string>

namespace
{
std::runtime_error sdlError(const char* operation)
{
    return std::runtime_error(std::string(operation) + ": " + SDL_GetError());
}
}

SdlPlatform::SdlPlatform(const char* title, int width, int height)
{
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        throw sdlError("Could not initialize SDL");
    }

    window_ = SDL_CreateWindow(title, width, height, SDL_WINDOW_RESIZABLE);
    if (window_ == nullptr)
    {
        SDL_Quit();
        throw sdlError("Could not create the window");
    }

    sdlRenderer_ = SDL_CreateRenderer(window_, nullptr);
    if (sdlRenderer_ == nullptr)
    {
        SDL_DestroyWindow(window_);
        SDL_Quit();
        throw sdlError("Could not create the SDL renderer");
    }

    texture_ = SDL_CreateTexture(
        sdlRenderer_,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        width,
        height);

    if (texture_ == nullptr)
    {
        SDL_DestroyRenderer(sdlRenderer_);
        SDL_DestroyWindow(window_);
        SDL_Quit();
        throw sdlError("Could not create the display texture");
    }
}

SdlPlatform::~SdlPlatform()
{
    SDL_DestroyTexture(texture_);
    SDL_DestroyRenderer(sdlRenderer_);
    SDL_DestroyWindow(window_);
    SDL_Quit();
}

int SdlPlatform::run(Renderer& renderer)
{
    bool isRunning = true;

    while (isRunning)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
            {
                isRunning = false;
            }
        }

        renderer.clear(18, 18, 24);
        present(renderer);
    }

    return 0;
}

void SdlPlatform::present(const Renderer& renderer)
{
    const int pitch = renderer.width() * static_cast<int>(sizeof(std::uint32_t));

    if (!SDL_UpdateTexture(texture_, nullptr, renderer.pixels(), pitch))
    {
        throw sdlError("Could not upload the CPU framebuffer");
    }

    if (!SDL_RenderClear(sdlRenderer_))
    {
        throw sdlError("Could not clear the window");
    }

    if (!SDL_RenderTexture(sdlRenderer_, texture_, nullptr, nullptr))
    {
        throw sdlError("Could not display the CPU framebuffer");
    }

    SDL_RenderPresent(sdlRenderer_);
}
