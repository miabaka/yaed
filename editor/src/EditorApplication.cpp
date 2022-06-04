#include "EditorApplication.hpp"

#include <fmt/format.h>
#include <imgui/imgui.h>
#include <imgui/imgui_stdlib.h>

bool EditorApplication::update(bool shouldClose) {
	if (ImGui::Begin("World Tree")) {
		const int defaultNodeFlags =
				ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_SpanAvailWidth
				| ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_OpenOnArrow;

		std::shared_ptr<const World> worldSelection = selectedWorldForInspector();
		std::shared_ptr<const Level> levelSelection = selectedLevelForInspector();

		for (const std::shared_ptr<World> &world: worlds()) {
			{
				const bool alreadySelected = (world == worldSelection);

				int nodeFlags = defaultNodeFlags;

				if (alreadySelected && !levelSelection)
					nodeFlags |= ImGuiTreeNodeFlags_Selected;

				const bool open = ImGui::TreeNodeEx(world.get(), nodeFlags, "%s", world->name().c_str());

				const bool justSelected = ImGui::IsItemActivated();

				if (justSelected) {
					selectWorldForInspector(world);
					selectLevelForInspector({});
				}

				if (!open)
					continue;
			}

			for (const std::shared_ptr<Level> &level: world->levels()) {
				ImGui::PushID(level.get());

				const bool alreadySelected = (level == levelSelection);
				const bool justSelected = ImGui::Selectable(level->name().c_str(), alreadySelected);

				if (justSelected) {
					selectWorldForInspector(world);
					selectLevelForInspector(level);
				}

				const bool doubleClicked =
						(ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left));

				if (doubleClicked)
					selectLevel(level);

				ImGui::PopID();
			}

			ImGui::TreePop();
		}
	}
	ImGui::End();

	if (ImGui::Begin("Inspector")) {
		const int headerFlags = ImGuiTreeNodeFlags_DefaultOpen;

		std::shared_ptr<World> world = selectedWorldForInspector();
		std::shared_ptr<Level> level = selectedLevelForInspector();

		if (world) {
			if (ImGui::CollapsingHeader("Game", headerFlags)) {
				std::string gameName = world->game()->name();
				ImGui::InputText("Name##Game", gameName, ImGuiInputTextFlags_ReadOnly);
			}

			if (ImGui::CollapsingHeader("World", headerFlags))
				ImGui::InputText("Name##World", world->name());
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
	ImGui::End();

	ImGui::Begin("Palette");
	ImGui::End();

	ImGui::Begin("Layers");
	ImGui::End();

	std::string viewportWindowTitle;

	{
		std::shared_ptr<Level> level = selectedLevel();

		if (level)
			viewportWindowTitle = fmt::format("{} ({})###Viewport", level->world()->name(), level->name());
		else
			viewportWindowTitle = "Viewport###Viewport";
	}

	ImGui::Begin(viewportWindowTitle.c_str());
	ImGui::End();

	return !shouldClose;
}

void EditorApplication::render() {

}

std::shared_ptr<World> EditorApplication::selectedWorldForInspector() {
	return _selectedWorldForInspector.lock();
}

void EditorApplication::selectWorldForInspector(std::shared_ptr<World> world) {
	_selectedWorldForInspector = world;
}

std::shared_ptr<Level> EditorApplication::selectedLevelForInspector() {
	return _selectedLevelForInspector.lock();
}

void EditorApplication::selectLevelForInspector(std::shared_ptr<Level> level) {
	_selectedLevelForInspector = level;
}