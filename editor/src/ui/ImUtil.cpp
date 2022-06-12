#include "ImUtil.hpp"

#include <glm/common.hpp>

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

void ImUtil::moveFromTitleBarOnly() {
	const glm::vec2 previousCursorPos = ImGui::GetCursorPos();
	const glm::vec2 contentRegionAvail = ImGui::GetContentRegionAvail();

	if (contentRegionAvail.x < 1 || contentRegionAvail.y < 1)
		return;

	ImGui::InvisibleButton("_moveFromTitleBarOnly", contentRegionAvail);

	ImGui::SetCursorPos(previousCursorPos);
}