#include "platform/sdl/SdlPlatform.h"
#include "renderer/Renderer.h"

#include <SDL3/SDL.h>

#include <exception>
#include <iostream>

int main(int, char**)
{
    constexpr int width = 800;
    constexpr int height = 600;

    try
    {
        Renderer renderer(width, height);
        SdlPlatform platform("Software Renderer", width, height);
        return platform.run(renderer);
    }
    catch (const std::exception& error)
    {
        std::cerr << "Application error: " << error.what() << '\n';
        return 1;
    }
}
