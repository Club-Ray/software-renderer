#include "App.h"

#include "assets/TextureLoader.h"

#include <filesystem>
#include <chrono>

App::App(const char* title, int width, int height)
    : renderer_(width, height),
      texture_(loadTexture(
          std::filesystem::path{SOFTWARE_RENDERER_ASSET_DIR} / "texture.jpg")),
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

        constexpr float speed = 200.0f; // Framebuffer pixels per second.
        const MovementInput input = platform_.movementInput();
        triangleX_ += input.x * speed * seconds;
        triangleY_ += input.y * speed * seconds;

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

	Triangle triangle;
	triangle.v[0].position = { static_cast<float>(centerX - halfWidth) + triangleX_,
        static_cast<float>(topY) + triangleY_ };
	triangle.v[0].uv = { 0.0f, 0.0f };
	triangle.v[0].color = 0xFFFF0000; // red color for the first vertex

	triangle.v[1].position = { static_cast<float>(centerX + halfWidth) + triangleX_,
		static_cast<float>(topY) + triangleY_ };
	triangle.v[1].uv = { 1.0f, 0.0f };
	triangle.v[1].color = 0xFF0000FF; // blue color for the second vertex

	triangle.v[2].position = { static_cast<float>(centerX) + triangleX_,
		static_cast<float>(topY + triangleHeight) + triangleY_ };
	triangle.v[2].uv = { 0.5f, 1.0f };
	triangle.v[2].color = 0xFF00FF00; // green color for the third vertex

    /*renderer_.fill_triangle(
        texture_,
        triangle);*/

	renderer_.fill_triangle(triangle);
}
