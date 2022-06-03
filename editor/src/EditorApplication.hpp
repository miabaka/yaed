#pragma once

#include "Editor.hpp"

class EditorApplication : public Editor {
public:
	bool update(bool shouldClose);

	void render();
};