#pragma once

#include "assets/Texture.h"

#include <filesystem>

[[nodiscard]] Texture loadTexture(const std::filesystem::path& path);
