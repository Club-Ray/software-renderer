# Software Renderer

A 3D software renderer built from scratch in C++. Rendering happens in a CPU-owned framebuffer; SDL 3 creates the window and displays that final image.

## Structure

```text
src/
|-- math/            GLM vector aliases and explicit 2D rotation helpers
|-- renderer/
|   |-- Mesh.h       Vertices, UVs, and indexed triangles
|   |-- Object.h     Mesh and object transform
|   |-- Transform.h  Scale, rotation, and translation data
|   |-- Texture.*    CPU texture storage and sampling
|   `-- Renderer.*   CPU framebuffer, lines, and triangle rasterization
|-- app/
|   |-- main.cpp     Entry point and error reporting
|   `-- App.*        Frame timing, update, draw, and presentation
`-- platform/sdl/   Window, keyboard events, framebuffer presentation, image loading
```

The app coordinates the CPU renderer and SDL platform. The renderer depends on
the math layer and builds without linking SDL. SDL receives the
completed framebuffer as pixels and a row pitch.

## Build

The project requires CMake 3.21 or newer and a C++17 compiler. SDL 3 and GLM 1.0.2
are Git submodules. GLM is consumed in header-only mode.

Clone the project with its submodules:

```powershell
git clone --recurse-submodules git@github.com:Club-Ray/software-renderer.git
```

If the project is already cloned, initialize its submodules:

```powershell
git submodule update --init --recursive
```

Then configure and build:

```powershell
cmake -S . -B build
cmake --build build
```

With a multi-configuration generator such as Visual Studio, run:

```powershell
.\build\Debug\software-renderer.exe
```

The application opens an 800 × 600 resizable window and presents the CPU
framebuffer. The current app clears and presents each frame while the 3D pipeline
is being built. The renderer can rasterize triangles already in framebuffer
coordinates. Close the window to exit.

## C++ style

Use four spaces and put block braces on their own lines. Use PascalCase for types
and lowerCamelCase for functions and variables. Add a trailing underscore to
private member names. Public data fields have no suffix. Use paths relative to
`src/` for project includes.
