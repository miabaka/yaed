#pragma once

#include <filesystem>

#include <glm/vec2.hpp>
#include <imgui/imgui.h>

class ImUtil {
public:
	static glm::vec2 adjustCursorForCentering(glm::vec2 size);

	static void centeredText(const char *text);

	static void centeredImage(ImTextureID texture, glm::vec2 size, glm::vec2 uv0 = {0, 0}, glm::vec2 uv1 = {1, 1});

	static void moveFromTitleBarOnly();

	static bool loadIniConfig(const std::filesystem::path &path);

	static void saveIniConfig(const std::filesystem::path &path);
};