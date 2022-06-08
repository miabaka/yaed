#include "ViewportWindow.hpp"

#include <fmt/format.h>
#include <imgui/imgui.h>

#include "../../core/World.hpp"

ViewportWindow::ViewportWindow()
		: BaseWindow("Viewport") {}

void ViewportWindow::setLevel(std::weak_ptr<Level> level) {
	if (level.expired())
		restoreDefaultTitle();

	_level = std::move(level);
}

void ViewportWindow::onDraw() {
	if (_level.expired())
		return;

	std::shared_ptr<Level> level = _level.lock();

	// TODO: move this line to some kind of rename event handler
	setTitle(fmt::format("{} ({})###Viewport", level->world()->name(), level->name()));
}