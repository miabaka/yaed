#pragma once

#include <memory>
#include <string>

#include <cute/dialogs/IDialogProvider.hpp>

#include "Editor.hpp"

class EditorApplication : public Editor {
public:
	EditorApplication();

	bool update(bool shouldClose);

	void render();

	std::shared_ptr<World> selectedWorldForInspector();

	void selectWorldForInspector(std::shared_ptr<World> world);

	std::shared_ptr<Level> selectedLevelForInspector();

	void selectLevelForInspector(std::shared_ptr<Level> level);

	void openWorld();

private:
	std::weak_ptr<World> _selectedWorldForInspector;
	std::weak_ptr<Level> _selectedLevelForInspector;
	std::unique_ptr<cute::dialogs::IDialogProvider> _dialogProvider;
};