#pragma once

#include "platform/sdl/SdlPlatform.h"
#include "renderer/Renderer.h"

class App
{
public:
    App(const char* title, int width, int height);

    int run();

private:
    void draw();

    Renderer renderer_;
    SdlPlatform platform_;
};
