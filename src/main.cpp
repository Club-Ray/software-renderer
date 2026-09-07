#include "App.h"

#include <exception>
#include <iostream>

int main(int, char**)
{
    constexpr int width = 800;
    constexpr int height = 600;

    try
    {
        App app("Software Renderer", width, height);
        return app.run();
    }
    catch (const std::exception& error)
    {
        std::cerr << "Application error: " << error.what() << '\n';
        return 1;
    }
}
