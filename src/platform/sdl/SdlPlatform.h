#pragma once

struct SDL_Renderer;
struct SDL_Texture;
struct SDL_Window;

class Renderer;

struct MovementInput
{
    int x = 0;
    int y = 0;
    int rot = 0; // deg
};

class SdlPlatform
{
public:
    SdlPlatform(const char* title, int width, int height);
    ~SdlPlatform();

    SdlPlatform(const SdlPlatform&) = delete;
    SdlPlatform& operator=(const SdlPlatform&) = delete;

    [[nodiscard]] bool processEvents();
    [[nodiscard]] MovementInput movementInput() const;
    [[nodiscard]] bool wireframeEnabled() const { return wireframeEnabled_; }
    void present(const Renderer& renderer);

private:
    bool wireframeEnabled_ = false;
    SDL_Window* window_ = nullptr;
    SDL_Renderer* sdlRenderer_ = nullptr;
    SDL_Texture* texture_ = nullptr;
};
