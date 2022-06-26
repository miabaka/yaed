#include "LayerListWindow.hpp"

#include <imgui/imgui.h>

LayerListWindow::LayerListWindow()
		: BaseWindow("layer_list", "Layers") {
	BaseWindow::setPadding({3, 2});
}

void LayerListWindow::setLevel(std::weak_ptr<Level> level) {
	_level = std::move(level);
}

static bool drawLayer(const Layer &layer, bool selected) {
	ImGui::PushID(&layer);

	bool justSelected = ImGui::Selectable("", selected, 0, {0, 32});

	ImGui::PopID();

	const glm::vec2 cursorPos = ImGui::GetCursorPos();

	ImGui::SetCursorPos(cursorPos + glm::vec2{1, -35});
	ImGui::Image({}, {40, 30});

	ImGui::SetCursorPos(cursorPos + glm::vec2{47, -36 + (32 - (int) ImGui::GetTextLineHeight()) / 2});
	ImGui::Text("%s", layer.name().c_str());

	ImGui::SetCursorPos(cursorPos);

	return justSelected;
}

void LayerListWindow::onDraw() {
	if (_level.expired())
		return;

	std::shared_ptr<Level> level = _level.lock();
	std::shared_ptr<Layer> selectedLayer = level->selectedLayer();

	const auto &layers = level->layers();

	for (auto it = layers.rbegin(); it < layers.rend(); it++) {
		if (drawLayer(**it, *it == selectedLayer))
			level->selectLayer(*it);
	}
}