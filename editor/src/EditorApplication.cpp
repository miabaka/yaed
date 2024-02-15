#include "EditorApplication.hpp"

#include <stdexcept>
#include <filesystem>
#include <fstream>
#include <sstream>

#include <imgui/imgui_stdlib.h>
#include <nlohmann/json.hpp>
#include <cute/shell/CuteShell.hpp>

#include "core/WorldUtil.hpp"
#include "ui/ImUtil.hpp"

namespace fs = std::filesystem;

using cute::shell::CuteShell;
using cute::shell::IFileDialog;

EditorApplication::EditorApplication()
		: _dialogProvider(CuteShell::createDialogProvider()) {
	const fs::path baseConfigPath = CuteShell::getAppDataPath() / "yaed";

	bool configDirectoryExists = fs::exists(baseConfigPath);

	if (!configDirectoryExists)
		configDirectoryExists = fs::create_directory(baseConfigPath);

	if (!configDirectoryExists)
		throw std::runtime_error("Can't create config directory");

	_configPath = baseConfigPath / "yaed.json";
	_imGuiConfigPath = baseConfigPath / "imgui.ini";

	loadConfig();

	_palette.setBrushSelectionSource(brushSelection());
	_viewport.setBrushSelectionSource(brushSelection());
}

EditorApplication::~EditorApplication() {
	saveConfig();
}

void EditorApplication::applyDefaultDockingLayout(ImGuiID dockSpaceId) {
	ImGui::DockBuilderRemoveNode(dockSpaceId);

	ImGuiID rootNode = ImGui::DockBuilderAddNode(dockSpaceId, ImGuiDockNodeFlags_DockSpace);
	ImGui::DockBuilderSetNodeSize(rootNode, ImGui::GetMainViewport()->Size);

	ImGuiID left;
	ImGuiID right;
	ImGui::DockBuilderSplitNode(rootNode, ImGuiDir_Left, 0.5f, &left, &right);

	ImGuiID leftOfRight;
	ImGuiID rightOfRight;
	ImGui::DockBuilderSplitNode(right, ImGuiDir_Right, 0.5f, &rightOfRight, &leftOfRight);

	ImGuiID topOfLeft;
	ImGuiID bottomOfLeft;
	ImGui::DockBuilderSplitNode(left, ImGuiDir_Down, 0.33f, &bottomOfLeft, &topOfLeft);

	ImGuiID topOfRightOfRight;
	ImGuiID bottomOfRightOfRight;
	ImGui::DockBuilderSplitNode(rightOfRight, ImGuiDir_Down, 0.85f, &bottomOfRightOfRight, &topOfRightOfRight);

	ImGui::DockBuilderDockWindow(_viewport.getIdWithHashPrefix().c_str(), leftOfRight);
	ImGui::DockBuilderDockWindow("###world_tree", topOfLeft);
	ImGui::DockBuilderDockWindow(_inspector.getIdWithHashPrefix().c_str(), bottomOfLeft);
	ImGui::DockBuilderDockWindow(_layers.getIdWithHashPrefix().c_str(), topOfRightOfRight);
	ImGui::DockBuilderDockWindow(_palette.getIdWithHashPrefix().c_str(), bottomOfRightOfRight);

	ImGui::DockBuilderSetNodeSize(left, {192, 1});
	ImGui::DockBuilderSetNodeSize(rightOfRight, {192, 1});

	ImGui::DockBuilderFinish(dockSpaceId);

	_inspector.setOpen(true);
	_layers.setOpen(true);
	_minimap.setOpen(false);
	_palette.setOpen(true);
	_viewport.setOpen(true);
	_worldTreeOpen = true;
}

bool EditorApplication::update(bool shouldClose) {
	const ImGuiID dockSpaceId = ImGui::DockSpaceOverViewport();

	if (_dockingLayoutMustBeReset) {
		applyDefaultDockingLayout(dockSpaceId);
		_dockingLayoutMustBeReset = false;
	}

	drawGlobalMenu();
	drawWorldTreeWindow();

	_inspector.draw();
	_layers.draw();
	_minimap.draw();
	_palette.draw();
	_viewport.draw();

	drawNewWorldDialog();

	return !shouldClose;
}

void EditorApplication::renderOffscreenViews() {
	_viewport.render();
}

static nlohmann::json readJsonFromFile(const fs::path &path) {
	std::ifstream file(path);

	nlohmann::json json;

	if (file)
		file >> json;

	return json;
}

static void writeJsonToFile(const nlohmann::json &json, const fs::path &path) {
	std::ofstream file(path);
	file << std::setw(4) << json;
}

void EditorApplication::loadConfig() {
	if (!ImUtil::loadIniConfig(_imGuiConfigPath))
		_dockingLayoutMustBeReset = true;

	nlohmann::json config = readJsonFromFile(_configPath);

	{
		const auto &recentFiles = config["common"]["recent_files"];

		if (recentFiles.is_array()) {
			for (const auto &file: recentFiles) {
				if (!file.is_string())
					continue;

				_recentlyOpened.pokePath(fs::u8path(file.get<std::string>()));
			}
		}
	}

	{
		auto &windowVisibility = config["ui"]["window_visibility"];

		auto applyValue = [&windowVisibility](const auto &key, bool &destination) {
			const auto &value = windowVisibility[key];

			if (value.is_boolean())
				destination = value.template get<bool>();
		};

		applyValue(_inspector.getId(), _inspector.isOpen());
		applyValue(_minimap.getId(), _minimap.isOpen());
		applyValue(_palette.getId(), _palette.isOpen());
		applyValue(_viewport.getId(), _viewport.isOpen());
		applyValue("world_tree", _worldTreeOpen);
	}
}

void EditorApplication::saveConfig() {
	ImUtil::saveIniConfig(_imGuiConfigPath);

	nlohmann::json config = readJsonFromFile(_configPath);

	{
		std::vector<std::string> recentFiles;

		for (const fs::path &path: _recentlyOpened.paths())
			recentFiles.push_back(path.u8string());

		config["common"]["recent_files"] = recentFiles;
	}

	{
		auto &windowVisibility = config["ui"]["window_visibility"];

		windowVisibility[_inspector.getId()] = _inspector.isOpen();
		windowVisibility[_layers.getId()] = _layers.isOpen();
		windowVisibility[_minimap.getId()] = _minimap.isOpen();
		windowVisibility[_palette.getId()] = _palette.isOpen();
		windowVisibility[_viewport.getId()] = _viewport.isOpen();
		windowVisibility["world_tree"] = _worldTreeOpen;
	}

	writeJsonToFile(config, _configPath);
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

	// TODO: handle exceptions and show error dialog
	BaseEditor::openWorld(selectedPath);
}

void EditorApplication::saveWorldAs(std::shared_ptr<World> world) {
	if (!world)
		return;

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

	BaseEditor::saveWorldAs(world, selectedPath);
}

void EditorApplication::saveSelectedWorldAs() {
	saveWorldAs(selectedWorld());
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

void EditorApplication::onWorldFileOperation(std::shared_ptr<World> world, const std::filesystem::path &path) {
	_recentlyOpened.pokePath(path);
}

void EditorApplication::drawGlobalMenu() {
	if (ImGui::BeginMainMenuBar()) {
		if (ImGui::BeginMenu("File")) {
			if (ImGui::MenuItem("New...", "ctrl+n"))
				_newWorldDialogMustBeOpen = true;

			ImGui::Separator();

			if (ImGui::MenuItem("Open...", "ctrl+o"))
				openWorld();

			if (ImGui::BeginMenu("Open Recent")) {
				if (drawRecentlyOpenedMenuItems()) {
					ImGui::Separator();

					if (ImGui::MenuItem("Clear This List"))
						_recentlyOpened.clear();
				} else {
					ImGui::MenuItem("(Empty)", {}, false, false);
				}


				ImGui::EndMenu();
			}

			ImGui::Separator();

			if (ImGui::MenuItem("Save", "ctrl+s", false, hasSelectedWorld()))
				saveSelectedWorld();

			if (ImGui::MenuItem("Save As...", "ctrl+shift+s", false, hasSelectedWorld()))
				saveSelectedWorldAs();

			ImGui::MenuItem("Save All", "ctrl+alt+s", false, false);

			ImGui::Separator();

			if (ImGui::MenuItem("Close", "ctrl+w", false, hasSelectedWorld()))
				closeSelectedWorld();

			ImGui::Separator();

			ImGui::MenuItem("Exit");

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Window")) {
			if (ImGui::MenuItem("Restore Default Layout"))
				_dockingLayoutMustBeReset = true;

			ImGui::Separator();

			ImGui::MenuItem("Inspector", {}, &_inspector.isOpen());
			ImGui::MenuItem("Layers", {}, &_layers.isOpen());
			ImGui::MenuItem("Minimap", {}, &_minimap.isOpen());
			ImGui::MenuItem("Palette", {}, &_palette.isOpen());
			ImGui::MenuItem("Viewport", {}, &_viewport.isOpen());
			ImGui::MenuItem("World Tree", {}, &_worldTreeOpen);
#ifndef NDEBUG
			ImGui::Separator();

			if (ImGui::BeginMenu("Dear ImGui")) {
				ImGui::MenuItem("Demo", {}, false);
				ImGui::MenuItem("Metrics/Debugger", {}, false);
				ImGui::MenuItem("Style Editor", {}, false);

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

bool EditorApplication::drawRecentlyOpenedMenuItems() {
	fs::path selectedPath;

	const auto &paths = _recentlyOpened.paths();

	for (auto it = paths.rbegin(); it < paths.rend(); it++) {
		const fs::path &path = *it;

		ImGui::PushID(&path);

		if (ImGui::MenuItem(path.filename().u8string().c_str()))
			selectedPath = path;

		if (ImGui::IsItemHovered())
			ImGui::SetTooltip("%s", path.u8string().c_str());

		ImGui::PopID();
	}

	if (!selectedPath.empty())
		BaseEditor::openWorld(selectedPath);

	return !paths.empty();
}

// TODO: split into multiple short functions
// TODO: create separate window class for this
void EditorApplication::drawWorldTreeWindow() {
	if (!_worldTreeOpen)
		return;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {1, 1});

	bool worldTreeIsOpen = ImGui::Begin("World Tree###world_tree", &_worldTreeOpen);

	ImGui::PopStyleVar();

	if (!worldTreeIsOpen) {
		ImGui::End();
		return;
	}

	const int defaultNodeFlags =
			ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_SpanFullWidth
			| ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_OpenOnArrow
			| ImGuiTreeNodeFlags_FramePadding;

	const int selectableFlags = ImGuiSelectableFlags_SpanAllColumns;

	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, {4, 2});

	std::shared_ptr<const World> worldSelection = selectedWorld();
	std::shared_ptr<const Level> levelSelection = selectedLevel();

	std::shared_ptr<World> worldToClose;

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
			const bool justMiddleClicked = ImGui::IsItemClicked(ImGuiMouseButton_Middle);
			const bool justSelected = justClicked && !_worldSelectionLockedByCollapsing;

			// TODO: trigger by mouse up, not down
			if (justMiddleClicked)
				worldToClose = world;

			if (justClicked)
				_worldSelectionLockedByCollapsing = false;

			if (justSelected)
				selectWorld(world);

			if (ImGui::BeginPopupContextItem()) {
				ImGui::MenuItem(world->nameOrFilename().c_str(), {}, false, false);

				ImGui::Separator();

				if (ImGui::MenuItem("New Level")) {
					std::shared_ptr<Level> level = WorldUtil::createLevel(world);

					if (level)
						selectLevel(level);
				}

				if (ImGui::BeginMenu("Autoname Levels")) {
					if (ImGui::MenuItem("All"))
						world->autonameLevels(World::AutonameMode::All);

					if (ImGui::MenuItem("Only Unnamed"))
						world->autonameLevels(World::AutonameMode::OnlyUnnamed);

					ImGui::EndMenu();
				}

				ImGui::Separator();

				{
					const fs::path &path = world->path();

					if (ImGui::MenuItem("Copy Path", {}, false, !path.empty()))
						ImGui::SetClipboardText(path.u8string().c_str());

					if (ImGui::IsItemHovered())
						ImGui::SetTooltip("%s", path.u8string().c_str());

					if (ImGui::MenuItem("Show In Explorer", {}, false, !path.empty()))
						CuteShell::revealFile(path);
				}

				ImGui::Separator();

				if (ImGui::MenuItem("Save"))
					saveWorld(world);

				if (ImGui::MenuItem("Save As..."))
					saveWorldAs(world);

				ImGui::Separator();

				if (ImGui::MenuItem("Close"))
					worldToClose = world;

				ImGui::EndPopup();
			}

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

				if (ImGui::BeginPopupContextItem()) {
					ImGui::MenuItem(level->name().c_str(), {}, false, false);

					ImGui::Separator();

					if (ImGui::MenuItem("Remove")) {
						level->markForRemoval();

						if (levelSelection == level)
							levelSelection = {};
					}

					ImGui::EndPopup();
				}

				ImGui::PopID();
			}

			ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 2);

			if (world->removeMarkedLevels() && !selectedLevel())
				selectFirstLevelOfWorld(world);
		}

		ImGui::TreePop();
	}

	ImGui::PopStyleVar();

	ImGui::End();

	if (worldToClose)
		closeWorld(worldToClose);
}

void EditorApplication::drawNewWorldDialog() {
	if (_newWorldDialogMustBeOpen) {
		_newWorldDialogMustBeOpen = false;
		_newWorldDialogState.reset();
		ImGui::OpenPopup("New");
	}

	ImVec2 center = ImGui::GetMainViewport()->GetCenter();
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, {0.5f, 0.5f});
	ImGui::SetNextWindowSize({268, 300}, ImGuiCond_Appearing);

	ImGui::PushStyleColor(ImGuiCol_PopupBg, ImGui::GetColorU32(ImGuiCol_WindowBg));

	bool newIsOpen = ImGui::BeginPopupModal("New");

	ImGui::PopStyleColor();

	if (!newIsOpen)
		return;

	const glm::vec2 size = ImGui::GetContentRegionAvail();

	ImGui::BeginChild("content", size - glm::vec2{0, 27});

	const float inputOffset = ImGui::GetCursorPosX() + 13 + 47;

	ImGui::Text("Common");

	ImGui::Indent(6);

	{
		ImGui::AlignTextToFramePadding();
		ImGui::Text("Name");

		ImGui::SameLine();
		ImGui::SetCursorPosX(inputOffset);
		ImGui::SetNextItemWidth(-7);
		ImGui::InputTextWithHint("###name", "None", &_newWorldDialogState.chosenName());
	}

	ImGui::Indent(-6);

	ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 6);
	ImGui::Text("Generation");

	ImGui::Indent(6);

	{
		ImGui::AlignTextToFramePadding();
		ImGui::Text("Game");

		ImGui::SameLine();
		ImGui::SetCursorPosX(inputOffset);
		ImGui::SetNextItemWidth(-7);

		if (ImGui::BeginCombo("###game", _newWorldDialogState.selectedGameRawName())) {
			for (const auto &it: games().games()) {
				const std::shared_ptr<IGame> &game = it.second;

				ImGui::PushID(game.get());

				if (ImGui::Selectable(game->name().c_str()))
					_newWorldDialogState.selectGame(game);

				ImGui::PopID();
			}

			ImGui::EndCombo();
		}

		ImGui::AlignTextToFramePadding();
		ImGui::Text("Factory");

		ImGui::SameLine();
		ImGui::SetCursorPosX(inputOffset);
		ImGui::SetNextItemWidth(-7);

		const bool canBeSelected = _newWorldDialogState.factoryCanBeSelected();

		ImGui::BeginDisabled(!canBeSelected);

		if (ImGui::BeginCombo("###factory", _newWorldDialogState.selectedFactoryRawName())) {
			for (const auto &it: worldFactories().factories()) {
				// TODO: game support check
				const std::shared_ptr<IWorldFactory> &factory = it.second;

				ImGui::PushID(factory.get());

				if (ImGui::Selectable(factory->name().c_str()))
					_newWorldDialogState.selectFactory(factory);

				ImGui::PopID();
			}

			ImGui::EndCombo();
		}

		ImGui::EndDisabled();
	}

	ImGui::Indent(-6);

	ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 6);
	ImGui::Text("Export");

	ImGui::Indent(6);

	{
		ImGui::AlignTextToFramePadding();
		ImGui::Text("Format");

		if (ImGui::IsItemHovered())
			ImGui::SetTooltip("Initial export format, can be changed later");

		ImGui::SameLine();
		ImGui::SetCursorPosX(inputOffset);
		ImGui::SetNextItemWidth(-7);

		const bool canBeSelected = _newWorldDialogState.formatCanBeSelected();

		ImGui::BeginDisabled(!canBeSelected);

		if (ImGui::BeginCombo("###format", _newWorldDialogState.selectedFormatRawName())) {
			if (ImGui::Selectable("None"))
				_newWorldDialogState.selectFormat({});

			for (const auto &it: worldFormats().formats()) {
				// TODO: game support check
				const std::shared_ptr<WorldFormat> &format = it.second;
				const WorldFormatInfo &info = format->info();

				ImGui::PushID(format.get());

				if (ImGui::Selectable(info.name().c_str()))
					_newWorldDialogState.selectFormat(format);

				ImGui::PopID();
			}

			ImGui::EndCombo();
		}

		ImGui::EndDisabled();
	}

	ImGui::Indent(-6);

	ImGui::EndChild();

	ImGui::Separator();

	ImGui::BeginDisabled(!_newWorldDialogState.valid());

	if (ImGui::Button("OK", {96, 0})) {
		const NewWorldDialogState &state = _newWorldDialogState;

		// TODO: don' t close dialog if world creation failed
		std::shared_ptr<World> world = createWorld(state.selectedGame(), state.selectedFactory(), state.chosenName());

		if (world) {
			std::shared_ptr<WorldFormat> format = _newWorldDialogState.selectedFormat();

			if (format)
				world->setExporter(format->exporter());
		}

		ImGui::CloseCurrentPopup();
	}

	ImGui::EndDisabled();

	ImGui::SameLine();

	if (ImGui::Button("Cancel", {96, 0}))
		ImGui::CloseCurrentPopup();

	ImGui::EndPopup();
}