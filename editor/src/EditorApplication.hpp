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

	void openWorld();

	void saveSelectedWorldAs();

private:
	std::unique_ptr<cute::dialogs::IDialogProvider> _dialogProvider;
};