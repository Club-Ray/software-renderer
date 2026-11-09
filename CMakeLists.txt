cmake_minimum_required(VERSION 3.21)

project(software_renderer LANGUAGES CXX)

set(SDL_TEST_LIBRARY OFF CACHE BOOL "" FORCE)
set(SDL_TESTS OFF CACHE BOOL "" FORCE)
set(SDL_EXAMPLES OFF CACHE BOOL "" FORCE)
set(SDL_INSTALL OFF CACHE BOOL "" FORCE)

if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/third_party/SDL/CMakeLists.txt")
    message(FATAL_ERROR
        "SDL submodule is missing. Run: git submodule update --init --recursive")
endif()

add_subdirectory(third_party/SDL EXCLUDE_FROM_ALL)

set(SOFTWARE_RENDERER_SOURCES
    src/App.cpp
    src/App.h
    src/assets/Texture.cpp
    src/assets/Texture.h
    src/assets/TextureLoader.cpp
    src/assets/TextureLoader.h
    src/assets/GothicGirl.cpp
    src/assets/GothicGirl.h
    src/main.cpp
    src/platform/sdl/SdlPlatform.cpp
    src/platform/sdl/SdlPlatform.h
    src/math/Math2D.h
    src/animation/Animation2D.cpp
    src/animation/Animation2D.h
    src/renderer/Mesh.h
    src/renderer/Object.h
    src/renderer/Triangle.h
    src/renderer/Renderer.cpp
    src/renderer/Renderer.h
)

add_executable(software-renderer ${SOFTWARE_RENDERER_SOURCES})

# Display sources and headers using the same folders as the source tree in IDEs.
source_group(
    TREE "${CMAKE_CURRENT_SOURCE_DIR}/src"
    FILES ${SOFTWARE_RENDERER_SOURCES}
)

target_compile_features(software-renderer PRIVATE cxx_std_17)

target_include_directories(software-renderer PRIVATE src)

# SDL already vendors stb_image; use that copy without exposing SDL internals
# to the renderer or application code.
target_include_directories(software-renderer SYSTEM PRIVATE
    third_party/SDL/src/video
)

target_compile_definitions(software-renderer PRIVATE
    SOFTWARE_RENDERER_ASSET_DIR="${CMAKE_CURRENT_SOURCE_DIR}/asset"
)

target_link_libraries(software-renderer PRIVATE SDL3::SDL3)

if(WIN32 AND TARGET SDL3::SDL3-shared)
    add_custom_command(TARGET software-renderer POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:SDL3::SDL3-shared>
            $<TARGET_FILE_DIR:software-renderer>
        VERBATIM
    )
endif()

if(MSVC)
    target_compile_options(software-renderer PRIVATE /W4 /permissive-)
else()
    target_compile_options(software-renderer PRIVATE -Wall -Wextra -Wpedantic)
endif()

option(SOFTWARE_RENDERER_BUILD_TESTS "Build small CPU rendering and animation checks" OFF)
if(SOFTWARE_RENDERER_BUILD_TESTS)
    enable_testing()
    add_executable(renderer-checks
        tests/RendererChecks.cpp
        src/renderer/Renderer.cpp
        src/assets/Texture.cpp
        src/assets/GothicGirl.cpp
        src/animation/Animation2D.cpp
    )
    target_compile_features(renderer-checks PRIVATE cxx_std_17)
    target_include_directories(renderer-checks PRIVATE src)
    add_test(NAME renderer-checks COMMAND renderer-checks)
endif()
