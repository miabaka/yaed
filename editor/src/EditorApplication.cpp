#include "EditorApplication.hpp"

#include <imgui/imgui.h>

bool EditorApplication::update(bool shouldClose) {
	if (ImGui::Begin("Baka"))
		ImGui::Button("Hewwo!");

	ImGui::End();

	return !shouldClose;
}

void EditorApplication::render() {
}