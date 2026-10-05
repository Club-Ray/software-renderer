#include "App.h"

#include "assets/TextureLoader.h"
#include "assets/GothicGirlMesh.h"

#include <filesystem>
#include <chrono>

App::App(const char* title, int width, int height)
    : renderer_(width, height),
      texture_(loadTexture(
          std::filesystem::path{SOFTWARE_RENDERER_ASSET_DIR} / "gothic_girl.png")),
      platform_(title, width, height)
{
    gothicGirl_.mesh_ = makeGothicGirlMesh();
    const float imageHeight = static_cast<float>(renderer_.height()) * 0.90f;
    const float imageWidth = imageHeight *
        static_cast<float>(texture_.width()) / static_cast<float>(texture_.height());
    gothicGirl_.scale_ = {imageWidth, imageHeight};
    gothicGirl_.position_ = {(renderer_.width() - imageWidth) * 0.5f,
                             (renderer_.height() - imageHeight) * 0.5f};
}

int App::run()
{
    auto previousFrame = std::chrono::steady_clock::now();
    while (platform_.processEvents())
    {
        const auto now = std::chrono::steady_clock::now();
        const float seconds = std::chrono::duration<float>(now - previousFrame).count();
        previousFrame = now;

        constexpr float speed = 200.0f; // Framebuffer pixels per second.
        const MovementInput input = platform_.movementInput();
        gothicGirl_.position_.x += input.x * speed * seconds;
        gothicGirl_.position_.y += input.y * speed * seconds;

        draw();
        platform_.present(renderer_);
    }

    return 0;
}

void App::draw()
{
    renderer_.clear(23, 17, 34);
    renderer_.draw(gothicGirl_, &texture_, platform_.wireframeEnabled());
}
