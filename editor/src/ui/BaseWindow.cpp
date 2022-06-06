#include "BaseWindow.hpp"

#include <imgui/imgui.h>

BaseWindow::BaseWindow(std::string name)
		: _name(std::move(name)) {}

void BaseWindow::draw() {
	if (!_open)
		return;

	if (ImGui::Begin(_name.c_str(), &_open))
		onDraw();

	ImGui::End();
}