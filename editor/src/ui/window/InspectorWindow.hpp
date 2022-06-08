#pragma once

#include <memory>

#include "../../core/World.hpp"
#include "../../core/Level.hpp"
#include "../BaseWindow.hpp"

class InspectorWindow : public BaseWindow {
public:
	InspectorWindow();

	void setWorld(std::weak_ptr<World> world);

	void setLevel(std::weak_ptr<Level> level);

protected:
	void onDraw() override;

private:
	std::weak_ptr<World> _world;
	std::weak_ptr<Level> _level;
};