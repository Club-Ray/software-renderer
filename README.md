# Software Renderer

A 3D software renderer built from scratch in C++. Rendering happens in a CPU-owned framebuffer; SDL 3 creates the window and displays that final image.

## Structure

```text
src/
|-- main.cpp          Entry point and error reporting
|-- App.*             Frame timing, input, update, and draw
|-- math/Math2D.h      Vec2 and rotation helpers
|-- renderer/
|   |-- Mesh.h        Vertex, UV, and indexed triangles (original bind geometry)
|   |-- Object.h      Mesh, optional posed vertices, and object transform
|   |-- Triangle.h    Three screen-space vertices ready to rasterize
|   `-- Renderer.*    Lines, transforms, triangle coverage, shading, alpha blending
|-- animation/        Bone influences, poses, and CPU skinning
|-- assets/           Gothic girl grid/idle pose, textures, and PNG/JPEG loading
`-- platform/sdl/     Window, keyboard events, and framebuffer presentation
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

The application opens an 800 x 600 resizable window showing the gothic girl PNG on a animated 32 × 48 grid mesh. The CPU renderer transforms and rasterizes its 3,072 textured triangles, blending transparent pixels over the background. Hold W, A, S, or D to move it. Press Q to toggle between textured rendering and wireframe mode. A gentle CPU idle loop sways the body, head, hair, arms, and skirt, using the pivots and smooth bone weights from stash bf48fdea. Each frame starts from the original bind positions, blends local bone rotation with the body, and then applies whole-body sway. UVs stay fixed; the animation is also visible in wireframe mode. Close the window to exit.

## Frame flow

`App::update` applies WASD movement, advances animation time, and builds the idle pose.
`skinMesh` reads the unchanged bind mesh and writes each unique vertex once into the
object's reusable posed vertex buffer. `App::draw` clears the framebuffer and draws.
The renderer uses posed vertices when present, or bind vertices for static objects.
It scales, rotates, and translates each vertex once, then assembles screen-space
`Triangle`s using the mesh indices. Colored and textured triangles share one
barycentric coverage loop; wireframe uses the same transformed vertices.

The gothic girl grid contains 1,617 unique vertices and 3,072 triangles. Animation
influences are separate from geometry, so static meshes do not need bone data.

## CPU checks

```powershell
cmake -S . -B build -DSOFTWARE_RENDERER_BUILD_TESTS=ON
cmake --build build --config Debug
ctest --test-dir build -C Debug --output-on-failure
```

## Bone weights

Each vertex's animation data contains a `std::vector<BoneWeight>`. Each entry pairs
one bone index with its weight, with no fixed connection limit or strongest-bone
selection. Weights are nonnegative and sum to one; zero-weight entries are ignored.
Index `-1` represents the unchanged local/body position. The gothic girl uses the
same smooth masks and idle rotations, stores all mask weights directly, adds the
remaining body weight, and normalizes the weights. Skinning transforms the original
position independently for each bone and adds those positions multiplied by their
weights. Whole-body sway is applied after that blend. These local bones do not
have a parent hierarchy.
