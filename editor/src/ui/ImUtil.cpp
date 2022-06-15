#include "ImUtil.hpp"

#include <fstream>
#include <sstream>

#include <glm/common.hpp>

namespace fs = std::filesystem;

glm::vec2 ImUtil::adjustCursorForCentering(glm::vec2 size) {
	const glm::vec2 initialCursorPos = ImGui::GetCursorPos();
	const glm::vec2 contentRegionSize = ImGui::GetContentRegionAvail();

	const auto offset = (contentRegionSize - size) * 0.5f;
	const auto clampedOffset = glm::max(glm::floor(offset), glm::vec2(0));

	ImGui::SetCursorPos(initialCursorPos + clampedOffset);

	return clampedOffset;
}

void ImUtil::centeredText(const char *text) {
	adjustCursorForCentering(ImGui::CalcTextSize(text));
	ImGui::Text("%s", text);
}

glm::vec2 ImUtil::centeredImage(ImTextureID texture, glm::vec2 size, glm::vec2 uv0, glm::vec2 uv1) {
	const auto offset = adjustCursorForCentering(size);

	ImGui::Image(texture, size, uv0, uv1);

	return offset;
}

bool ImUtil::loadIniConfig(const fs::path &path) {
	std::ifstream file(path);

	std::stringstream ss;
	ss << file.rdbuf();

	if (!file.good())
		return false;

	const std::string &iniConfig = ss.str();
	ImGui::LoadIniSettingsFromMemory(iniConfig.data(), iniConfig.size());

	return true;
}

void ImUtil::saveIniConfig(const fs::path &path) {
	auto &io = ImGui::GetIO();

	if (!io.WantSaveIniSettings)
		return;

	io.WantSaveIniSettings = false;

	std::ofstream file(path);
	file << ImGui::SaveIniSettingsToMemory();
}