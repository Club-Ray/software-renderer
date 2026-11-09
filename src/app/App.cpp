#include "app/App.h"

#include "platform/sdl/TextureLoader.h"

#include <chrono>
#include <filesystem>

App::App(const char* title, int width, int height)
    : renderer_(width, height),
      texture_(loadTexture(
          std::filesystem::path{SOFTWARE_RENDERER_ASSET_DIR} / "gothic_girl.png")),
      platform_(title, width, height)
{
}

int App::run()
{
    auto previousFrame = std::chrono::steady_clock::now();
    while (platform_.processEvents())
    {
        const auto now = std::chrono::steady_clock::now();
        const float seconds = std::chrono::duration<float>(now - previousFrame).count();
        previousFrame = now;

        update(seconds);
        draw();
        const int pitch = renderer_.width() * static_cast<int>(sizeof(std::uint32_t));
        platform_.present(renderer_.pixels(), pitch);
    }

    return 0;
}

void App::update(float /*seconds*/)
{
}

void App::draw()
{
    renderer_.clear(23, 17, 34);
}
