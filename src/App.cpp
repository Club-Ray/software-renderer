#include "App.h"

App::App(const char* title, int width, int height)
    : renderer_(width, height),
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
        centerX - halfWidth, topY, 0xFFFF0000,
        centerX + halfWidth, topY, 0xFF00FF00,
        centerX, topY + triangleHeight, 0xFF0000FF);
}
