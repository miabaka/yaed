#pragma once

#include <memory>
#include <string>

#include <imgui/imgui_internal.h>
#include <cute/shell/IDialogProvider.hpp>

#include "core/RecentlyOpenedManager.hpp"
#include "ui/window/InspectorWindow.hpp"
#include "ui/window/LayerListWindow.hpp"
#include "ui/window/MinimapWindow.hpp"
#include "ui/window/PaletteWindow.hpp"
#include "ui/window/ViewportWindow.hpp"
#include "Editor.hpp"
#include "ui/NewWorldDialogState.hpp"

class EditorApplication : public Editor {
public:
	EditorApplication();

	~EditorApplication() override;

	bool update(bool shouldClose);

	void render();

	void openWorld();

	void saveWorldAs(std::shared_ptr<World> world);

	void saveSelectedWorldAs();

protected:
	void onWorldSelectionChange(std::shared_ptr<World> world) override;

	void onLevelSelectionChange(std::shared_ptr<Level> level) override;

	void onWorldFileOperation(std::shared_ptr<World> world, const std::filesystem::path &path) override;

private:
	std::unique_ptr<cute::shell::IDialogProvider> _dialogProvider;
	// TODO: make some kind of window manager
	InspectorWindow _inspector;
	LayerListWindow _layers;
	MinimapWindow _minimap;
	PaletteWindow _palette;
	ViewportWindow _viewport;
	bool _worldSelectionLockedByCollapsing = false;
	bool _dockingLayoutMustBeReset = false;
	bool _worldTreeOpen = true;
	bool _newWorldDialogMustBeOpen = false;
	RecentlyOpenedManager _recentlyOpened;
	std::filesystem::path _configPath;
	std::filesystem::path _imGuiConfigPath;
	NewWorldDialogState _newWorldDialogState;

	void loadConfig();

	void saveConfig();

	void applyDefaultDockingLayout(ImGuiID dockSpaceId);

	void drawGlobalMenu();

	bool drawRecentlyOpenedMenuItems();

	void drawNewWorldDialog();

	// TODO: create separate class for this
	void drawWorldTreeWindow();
};