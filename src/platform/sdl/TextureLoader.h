#pragma once

#include "renderer/Texture.h"

#include <filesystem>

[[nodiscard]] Texture loadTexture(const std::filesystem::path& path);
