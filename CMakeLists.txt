cmake_minimum_required(VERSION 3.21)

project(software_renderer LANGUAGES CXX)

foreach(dependency IN ITEMS SDL glm)
    if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/third_party/${dependency}/CMakeLists.txt")
        message(FATAL_ERROR
            "${dependency} submodule is missing. Run: git submodule update --init --recursive")
    endif()
endforeach()

set(SDL_TEST_LIBRARY OFF CACHE BOOL "" FORCE)
set(SDL_TESTS OFF CACHE BOOL "" FORCE)
set(SDL_EXAMPLES OFF CACHE BOOL "" FORCE)
set(SDL_INSTALL OFF CACHE BOOL "" FORCE)
add_subdirectory(third_party/SDL EXCLUDE_FROM_ALL)

set(GLM_BUILD_LIBRARY OFF)
add_subdirectory(third_party/glm EXCLUDE_FROM_ALL)

add_library(renderer_math INTERFACE)
target_sources(renderer_math INTERFACE
    src/math/Math2D.h
    src/math/Math3D.h
)
target_include_directories(renderer_math INTERFACE src)
target_compile_features(renderer_math INTERFACE cxx_std_17)
target_link_libraries(renderer_math INTERFACE glm::glm-header-only)

add_library(renderer STATIC
    src/renderer/Mesh.h
    src/renderer/Object.h
    src/renderer/Renderer.cpp
    src/renderer/Renderer.h
    src/renderer/Texture.cpp
    src/renderer/Texture.h
    src/renderer/Transform.h
)
target_link_libraries(renderer PUBLIC renderer_math)

add_library(renderer_platform STATIC
    src/platform/sdl/SdlPlatform.cpp
    src/platform/sdl/SdlPlatform.h
    src/platform/sdl/TextureLoader.cpp
    src/platform/sdl/TextureLoader.h
)
target_link_libraries(renderer_platform PRIVATE renderer SDL3::SDL3)
target_include_directories(renderer_platform SYSTEM PRIVATE third_party/SDL/src/video)

add_executable(software-renderer
    src/app/main.cpp
    src/app/App.cpp
    src/app/App.h
)
target_link_libraries(software-renderer PRIVATE renderer renderer_platform)
target_compile_definitions(software-renderer PRIVATE
    SOFTWARE_RENDERER_ASSET_DIR="${CMAKE_CURRENT_SOURCE_DIR}/asset"
)

if(WIN32 AND TARGET SDL3::SDL3-shared)
    add_custom_command(TARGET software-renderer POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:SDL3::SDL3-shared>
            $<TARGET_FILE_DIR:software-renderer>
        VERBATIM
    )
endif()

if(CMAKE_GENERATOR MATCHES "^Visual Studio")
    # The build targets above still explicitly list the files they compile.
    file(GLOB_RECURSE project_sources CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    )
    source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/src" FILES ${project_sources})
    add_custom_target(project-sources SOURCES ${project_sources})
endif()

foreach(target IN ITEMS renderer renderer_platform software-renderer)
    if(MSVC)
        target_compile_options(${target} PRIVATE /W4 /permissive-)
    else()
        target_compile_options(${target} PRIVATE -Wall -Wextra -Wpedantic)
    endif()
endforeach()
