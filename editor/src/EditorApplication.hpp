#pragma once

#include <memory>
#include <string>

#include <cute/dialogs/IDialogProvider.hpp>

#include "ui/window/PaletteWindow.hpp"
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

private:
	std::unique_ptr<cute::dialogs::IDialogProvider> _dialogProvider;
	PaletteWindow _paletteWindow;
};