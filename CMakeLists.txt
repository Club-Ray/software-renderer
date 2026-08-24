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
    src/main.cpp
    src/platform/sdl/SdlPlatform.cpp
    src/platform/sdl/SdlPlatform.h
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
