#include "PaletteWindow.hpp"

#include <imgui/imgui.h>

PaletteWindow::PaletteWindow()
		: BaseWindow("Palette") {}

void PaletteWindow::onDraw() {
	ImGui::Text("H-hewwo");
}