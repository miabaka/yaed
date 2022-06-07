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
	void onLevelSelectionChange(std::shared_ptr<Level> level) override;

private:
	std::unique_ptr<cute::dialogs::IDialogProvider> _dialogProvider;
	PaletteWindow _paletteWindow;
};