#pragma once

#include "assets/Texture.h"
#include "animation/Animation2D.h"
#include "platform/sdl/SdlPlatform.h"
#include "renderer/Renderer.h"
#include "renderer/Object.h"

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
    Object gothicGirl_;
    std::vector<VertexInfluence> influences_;
    float animationTime_ = 0.0f;
};
