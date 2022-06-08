#pragma once

#include <memory>
#include <string>

#include <cute/dialogs/IDialogProvider.hpp>

#include "ui/window/InspectorWindow.hpp"
#include "ui/window/LayerListWindow.hpp"
#include "ui/window/PaletteWindow.hpp"
#include "ui/window/ViewportWindow.hpp"
#include "Editor.hpp"

class EditorApplication : public Editor {
public:
	EditorApplication();

	bool update(bool shouldClose);

	void render();

	void openWorld();

	void saveSelectedWorldAs();

protected:
	void onWorldSelectionChange(std::shared_ptr<World> world) override;

	void onLevelSelectionChange(std::shared_ptr<Level> level) override;

private:
	std::unique_ptr<cute::dialogs::IDialogProvider> _dialogProvider;
	InspectorWindow _inspector;
	LayerListWindow _layers;
	PaletteWindow _palette;
	ViewportWindow _viewport;

	void drawGlobalMenu();
};