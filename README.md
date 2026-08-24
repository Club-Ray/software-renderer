# Software Renderer

A 3D software renderer built from scratch in C++. Rendering happens in a CPU-owned framebuffer; SDL 3 creates the window and displays that final image.

## Structure

```text
src/
|-- renderer/       CPU rendering and framebuffer code
`-- platform/sdl/   Window, events, and framebuffer presentation
```

## Build

The project requires CMake 3.21 or newer and a C++17 compiler. SDL 3 is included as a Git submodule, so no system-wide SDL installation is needed.

Clone the project with its submodules:

```powershell
git clone --recurse-submodules git@github.com:Club-Ray/software-renderer.git
```

If the project is already cloned, initialize SDL once:

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

The application opens an 800 x 600 resizable window containing the blank, dark CPU framebuffer. Close the window to exit.
