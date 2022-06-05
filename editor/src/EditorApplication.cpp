#include "EditorApplication.hpp"

#include <filesystem>

#include <fmt/format.h>
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <imgui/imgui_stdlib.h>
#include <cute/dialogs/CuteDialogs.hpp>

namespace fs = std::filesystem;

using cute::dialogs::CuteDialogs;
using cute::dialogs::IFileDialog;

EditorApplication::EditorApplication()
		: _dialogProvider(CuteDialogs::createDialogProvider()) {}

bool EditorApplication::update(bool shouldClose) {
	if (ImGui::BeginMainMenuBar()) {
		if (ImGui::BeginMenu("File")) {
			ImGui::MenuItem("New...", "ctrl+n");

			ImGui::Separator();

			if (ImGui::MenuItem("Open...", "ctrl+o"))
				openWorld();

			if (ImGui::BeginMenu("Open Recent")) {
				ImGui::MenuItem("Clear Recently Opened");
				ImGui::EndMenu();
			}

			ImGui::Separator();

			if (ImGui::MenuItem("Save", "ctrl+s"))
				saveSelectedWorld();

			if (ImGui::MenuItem("Save As...", "ctrl+shift+s"))
				saveSelectedWorldAs();

			ImGui::MenuItem("Save All", "ctrl+alt+s");

			ImGui::Separator();

			if (ImGui::MenuItem("Close", "ctrl+w"))
				closeSelectedWorld();

			ImGui::Separator();

			ImGui::MenuItem("Exit");

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Window")) {
			ImGui::MenuItem("Restore Default Layout");

			ImGui::Separator();

			ImGui::MenuItem("Inspector", {}, true);
			ImGui::MenuItem("Layers", {}, true);
			ImGui::MenuItem("Palette", {}, true);
			ImGui::MenuItem("Viewport", {}, true);
			ImGui::MenuItem("World Tree", {}, true);
#ifndef NDEBUG
			ImGui::Separator();

			if (ImGui::BeginMenu("Dear ImGui")) {
				ImGui::MenuItem("Demo", nullptr, false);
				ImGui::MenuItem("Metrics/Debugger", nullptr, false);
				ImGui::MenuItem("Style Editor", nullptr, false);

				ImGui::EndMenu();
			}
#endif
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Help")) {
			ImGui::MenuItem("About");
			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}

	if (ImGui::Begin("World Tree")) {
		const int defaultNodeFlags =
				ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_SpanAvailWidth
				| ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_OpenOnArrow;

		std::shared_ptr<const World> worldSelection = selectedWorld();
		std::shared_ptr<const Level> levelSelection = selectedLevel();

		for (const std::shared_ptr<World> &world: worlds()) {
			{
				const bool alreadySelected = (world == worldSelection);

				int nodeFlags = defaultNodeFlags;

				// Highlight node as selected if:
				// - world is directly selected
				// - world is collapsed and one of its levels is selected
				{
					const ImGuiID nextNodeId = ImGui::GetCurrentWindow()->GetID(world.get());

					const bool exists = ImGui::GetStateStorage()->GetInt(nextNodeId, -1) != -1;
					const bool previousOpen = ImGui::TreeNodeBehaviorIsOpen(nextNodeId) || !exists;

					if (alreadySelected && !(levelSelection && previousOpen))
						nodeFlags |= ImGuiTreeNodeFlags_Selected;
				}

				const bool open = ImGui::TreeNodeEx(world.get(), nodeFlags, "%s", world->name().c_str());

				const bool justSelected = ImGui::IsItemActivated() && !ImGui::IsItemToggledOpen();

				if (justSelected)
					selectWorld(world);

				if (!open)
					continue;
			}

			for (const std::shared_ptr<Level> &level: world->levels()) {
				ImGui::PushID(level.get());

				const bool alreadySelected = (level == levelSelection);
				const bool justSelected = ImGui::Selectable(level->name().c_str(), alreadySelected);

				if (justSelected)
					selectLevel(level);

				ImGui::PopID();
			}

			ImGui::TreePop();
		}
	}
	ImGui::End();

	if (ImGui::Begin("Inspector")) {
		const int headerFlags = ImGuiTreeNodeFlags_DefaultOpen;

		std::shared_ptr<World> world = selectedWorld();
		std::shared_ptr<Level> level = selectedLevel();

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

void EditorApplication::openWorld() {
	std::unique_ptr<IFileDialog> dialog = _dialogProvider->createFileDialog(IFileDialog::Type::Open);

	for (const auto &[_, format]: worldFormatManager().formats()) {
		const WorldFormatInfo &info = format->info();
		dialog->addExtensionFilter(info.name(), "*." + info.fileExtension());
	}

	dialog->addExtensionFilter(L"All files", "*.*");

	const fs::path selectedPath = dialog->show();

	if (selectedPath.empty())
		return;

	BaseEditor::openWorld(selectedPath);
}

void EditorApplication::saveSelectedWorldAs() {
	std::shared_ptr<Level> level = selectedLevel();

	if (!level)
		return;

	std::shared_ptr<World> world = level->world();
	const std::string &gameId = world->game()->id();

	std::unique_ptr<IFileDialog> dialog = _dialogProvider->createFileDialog(IFileDialog::Type::Save);

	for (const auto &[_, format]: worldFormatManager().formats()) {
		const WorldFormatInfo &info = format->info();
		std::shared_ptr<const BaseWorldExporter> exporter = format->exporter();

		if (!(exporter && exporter->gameIsSupported(gameId)))
			continue;

		dialog->addExtensionFilter(info.name(), "*." + info.fileExtension());
	}

	const fs::path selectedPath = dialog->show();

	if (selectedPath.empty())
		return;

	BaseEditor::saveSelectedWorldAs(selectedPath);
}