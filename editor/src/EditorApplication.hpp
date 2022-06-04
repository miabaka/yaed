#pragma once

#include <memory>
#include <string>
#include "Editor.hpp"

class EditorApplication : public Editor {
public:
	bool update(bool shouldClose);

	void render();

	std::shared_ptr<World> selectedWorldForInspector();

	void selectWorldForInspector(std::shared_ptr<World> world);

	std::shared_ptr<Level> selectedLevelForInspector();

	void selectLevelForInspector(std::shared_ptr<Level> level);

private:
	std::weak_ptr<World> _selectedWorldForInspector;
	std::weak_ptr<Level> _selectedLevelForInspector;
};