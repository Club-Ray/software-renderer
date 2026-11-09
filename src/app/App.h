#pragma once

#include "platform/sdl/SdlPlatform.h"
#include "renderer/Object.h"
#include "renderer/Renderer.h"
#include "renderer/Texture.h"

class App
{
public:
    App(const char* title, int width, int height);

    int run();

private:
    void update(float seconds);
    void draw();

    Renderer renderer_;
    Texture texture_;
    SdlPlatform platform_;
    Object object_;
};
