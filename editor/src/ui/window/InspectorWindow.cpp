#include "InspectorWindow.hpp"

#include <imgui/imgui.h>
#include <imgui/imgui_stdlib.h>

InspectorWindow::InspectorWindow()
		: BaseWindow("Inspector") {}

void InspectorWindow::setWorld(std::weak_ptr<World> world) {
	_world = std::move(world);
}

void InspectorWindow::setLevel(std::weak_ptr<Level> level) {
	_level = std::move(level);
}

void InspectorWindow::onDraw() {
	const int headerFlags = ImGuiTreeNodeFlags_DefaultOpen;

	std::shared_ptr<World> world = _world.lock();
	std::shared_ptr<Level> level = _level.lock();

	if (world) {
		if (ImGui::CollapsingHeader("Game", headerFlags)) {
			std::string gameName = world->game()->name();
			ImGui::InputText("Name##Game", gameName, ImGuiInputTextFlags_ReadOnly);
		}

		if (ImGui::CollapsingHeader("World", headerFlags))
			ImGui::InputTextWithHint("Name##World", world->filename().c_str(), world->name());
	}

	if (level && ImGui::CollapsingHeader("Level", headerFlags)) {
		std::shared_ptr<LevelSkin> levelSkin = level->skin();

		ImGui::InputText("Name##Level", level->name());

		if (ImGui::BeginCombo("Skin", levelSkin->name().c_str())) {
			for (auto &[_, skin]: world->game()->levelSkins()) {
				const bool alreadySelected = (skin == levelSkin);
				const bool selected = ImGui::Selectable(skin->name().c_str(), alreadySelected);

				if (!selected)
					continue;

				level->setSkin(skin);
			}

			ImGui::EndCombo();
		}
	}
}