#pragma once

struct SDL_Renderer;
struct SDL_Texture;
struct SDL_Window;

class Renderer;

class SdlPlatform
{
public:
    SdlPlatform(const char* title, int width, int height);
    ~SdlPlatform();

    SdlPlatform(const SdlPlatform&) = delete;
    SdlPlatform& operator=(const SdlPlatform&) = delete;

    [[nodiscard]] bool processEvents();
    void present(const Renderer& renderer);

private:
    SDL_Window* window_ = nullptr;
    SDL_Renderer* sdlRenderer_ = nullptr;
    SDL_Texture* texture_ = nullptr;
};
