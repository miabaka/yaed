#include "PaletteWindow.hpp"

#include <imgui/imgui.h>

PaletteWindow::PaletteWindow()
		: BaseWindow("Palette") {}

void PaletteWindow::setTemplate(std::weak_ptr<const PaletteTemplate> paletteTemplate) {
	_template = std::move(paletteTemplate);
}

void PaletteWindow::onDraw() {
	if (_template.expired())
		return;

	ImGui::Text("Yass");
}