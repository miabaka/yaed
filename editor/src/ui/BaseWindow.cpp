#include "BaseWindow.hpp"

#include <imgui/imgui.h>

BaseWindow::BaseWindow(std::string name)
		: _name(std::move(name)) {}

void BaseWindow::draw() {
	if (!_open)
		return;

	onBeginPre();

	bool collapsed = !ImGui::Begin(_name.c_str(), &_open);

	onBeginPost();

	if (!collapsed)
		onDraw();

	ImGui::End();
}

void BaseWindow::onBeginPre() {}

void BaseWindow::onBeginPost() {}

void BaseWindow::onDraw() {}