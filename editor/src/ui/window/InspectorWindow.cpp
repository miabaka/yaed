#include "InspectorWindow.hpp"

#include <imgui/imgui.h>
#include <imgui/imgui_stdlib.h>

InspectorWindow::InspectorWindow()
		: BaseWindow("inspector", "Inspector") {}

void InspectorWindow::setWorld(std::weak_ptr<World> world) {
	_world = std::move(world);
}

void InspectorWindow::setLevel(std::weak_ptr<Level> level) {
	_level = std::move(level);
}

void InspectorWindow::onDraw() {
	std::shared_ptr<World> world = _world.lock();
	std::shared_ptr<Level> level = _level.lock();

	const float inputOffset = ImGui::GetCursorPosX() + 39;
	const float inputWidth = -6;

	if (world) {
		ImGui::Text("World");

		ImGui::Indent(6);

		ImGui::AlignTextToFramePadding();
		ImGui::Text("Name");

		ImGui::SameLine();
		ImGui::SetCursorPosX(inputOffset);
		ImGui::SetNextItemWidth(inputWidth);
		ImGui::InputTextWithHint("###world_name", world->filename().c_str(), world->name());

		ImGui::Indent(-6);

		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 6);
	}

	if (level) {
		std::shared_ptr<LevelSkin> levelSkin = level->skin();

		ImGui::Text("Level");

		ImGui::Indent(6);

		ImGui::AlignTextToFramePadding();
		ImGui::Text("Name");

		ImGui::SameLine();
		ImGui::SetCursorPosX(inputOffset);
		ImGui::SetNextItemWidth(inputWidth);
		ImGui::InputText("###level_name", level->name());

		ImGui::AlignTextToFramePadding();
		ImGui::Text("Skin");

		ImGui::SameLine();
		ImGui::SetCursorPosX(inputOffset);
		ImGui::SetNextItemWidth(inputWidth);

		if (ImGui::BeginCombo("###level_skin", levelSkin->name().c_str())) {
			for (auto &[_, skin]: world->game()->levelSkins()) {
				const bool alreadySelected = (skin == levelSkin);
				const bool selected = ImGui::Selectable(skin->name().c_str(), alreadySelected);

				if (!selected)
					continue;

				level->setSkin(skin);
			}

			ImGui::EndCombo();
		}

		ImGui::Indent(-6);
	}
}