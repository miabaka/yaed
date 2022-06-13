#include "EditorApplication.hpp"

#include <filesystem>

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
	drawGlobalMenu();

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {1, 1});

	if (ImGui::Begin("World Tree")) {
		const int defaultNodeFlags =
				ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_SpanFullWidth
				| ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_OpenOnArrow
				| ImGuiTreeNodeFlags_FramePadding;

		const int selectableFlags = ImGuiSelectableFlags_SpanAllColumns;

		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, {4, 2});

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

				ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, {0, 0});

				const bool open = ImGui::TreeNodeEx(world.get(), nodeFlags, "%s", world->nameOrFilename().c_str());

				ImGui::PopStyleVar();

				if (ImGui::IsItemToggledOpen())
					_worldSelectionLockedByCollapsing = true;

				const bool justClicked = ImGui::IsItemDeactivated() && ImGui::IsItemHovered();
				const bool justSelected = justClicked && !_worldSelectionLockedByCollapsing;

				if (justClicked)
					_worldSelectionLockedByCollapsing = false;

				if (justSelected)
					selectWorld(world);

				if (!open)
					continue;
			}

			if (!world->levels().empty()) {
				ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 2);

				for (const std::shared_ptr<Level> &level: world->levels()) {
					ImGui::PushID(level.get());

					const bool alreadySelected = (level == levelSelection);
					const bool justSelected =
							ImGui::Selectable(level->name().c_str(), alreadySelected, selectableFlags);

					if (justSelected)
						selectLevel(level);

					ImGui::PopID();
				}

				ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 2);
			}

			ImGui::TreePop();
		}

		ImGui::PopStyleVar();
	}
	ImGui::End();

	ImGui::PopStyleVar();

	_inspector.draw();
	_layers.draw();
	_minimap.draw();
	_palette.draw();
	_viewport.draw();

	return !shouldClose;
}

void EditorApplication::render() {
	_viewport.render();
}

void EditorApplication::openWorld() {
	std::unique_ptr<IFileDialog> dialog = _dialogProvider->createFileDialog(IFileDialog::Type::Open);

	for (const auto &[_, format]: worldFormats().formats()) {
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

	for (const auto &[_, format]: worldFormats().formats()) {
		const WorldFormatInfo &info = format->info();
		std::shared_ptr<const IWorldExporter> exporter = format->exporter();

		if (!(exporter && exporter->gameIsSupported(gameId)))
			continue;

		dialog->addExtensionFilter(info.name(), "*." + info.fileExtension());
	}

	const fs::path selectedPath = dialog->show();

	if (selectedPath.empty())
		return;

	BaseEditor::saveSelectedWorldAs(selectedPath);
}

void EditorApplication::onWorldSelectionChange(std::shared_ptr<World> world) {
	_inspector.setWorld(world);
}

void EditorApplication::onLevelSelectionChange(std::shared_ptr<Level> level) {
	_inspector.setLevel(level);
	_layers.setLevel(level);
	_viewport.setLevel(*this, level);
	_palette.setLevel(*this, level);
}

void EditorApplication::drawGlobalMenu() {
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

			if (ImGui::MenuItem("Save", "ctrl+s", false, hasSelectedWorld()))
				saveSelectedWorld();

			if (ImGui::MenuItem("Save As...", "ctrl+shift+s", false, hasSelectedWorld()))
				saveSelectedWorldAs();

			ImGui::MenuItem("Save All", "ctrl+alt+s");

			ImGui::Separator();

			if (ImGui::MenuItem("Close", "ctrl+w", false, hasSelectedWorld()))
				closeSelectedWorld();

			ImGui::Separator();

			ImGui::MenuItem("Exit");

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Window")) {
			ImGui::MenuItem("Restore Default Layout");

			ImGui::Separator();

			ImGui::MenuItem("Inspector", {}, &_inspector.isOpen());
			ImGui::MenuItem("Layers", {}, &_layers.isOpen());
			ImGui::MenuItem("Minimap", {}, &_minimap.isOpen());
			ImGui::MenuItem("Palette", {}, &_palette.isOpen());
			ImGui::MenuItem("Viewport", {}, &_viewport.isOpen());
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
}