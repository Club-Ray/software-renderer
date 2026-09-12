#include "App.h"

#include "assets/TextureLoader.h"

#include <filesystem>

App::App(const char* title, int width, int height)
    : renderer_(width, height),
      texture_(loadTexture(
          std::filesystem::path{SOFTWARE_RENDERER_ASSET_DIR} / "texture.jpg")),
      platform_(title, width, height)
{
}

int App::run()
{
    while (platform_.processEvents())
    {
        draw();
        platform_.present(renderer_);
    }

    return 0;
}

void App::draw()
{
    renderer_.clear(18, 18, 24);

    const int centerX = renderer_.width() / 2;
    const int centerY = renderer_.height() / 2;
    const int halfWidth = renderer_.width() / 4;
    const int triangleHeight = renderer_.height() / 2;
    const int topY = centerY - triangleHeight / 3;

    renderer_.fill_triangle(
        texture_,
        centerX - halfWidth, topY, UV{0.0f, 0.0f},
        centerX + halfWidth, topY, UV{1.0f, 0.0f},
        centerX, topY + triangleHeight, UV{0.0f, 1.0f});
}
