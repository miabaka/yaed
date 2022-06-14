#include "MinimapWindow.hpp"

#include "../ImUtil.hpp"

MinimapWindow::MinimapWindow()
		: BaseWindow("minimap", "Minimap") {
	BaseWindow::setOpen(false);
}

void MinimapWindow::onDraw() {
	ImUtil::moveFromTitleBarOnly();
}