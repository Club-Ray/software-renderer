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
    renderer_.DrawLine(100, 100, 200, 100, 0xFFFFFFFF);
    renderer_.DrawLine(200, 100, 150, 200, 0xFFFFFFFF);
    renderer_.DrawLine(150, 200, 100, 100, 0xFFFFFFFF);
}
