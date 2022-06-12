#include "BaseWindow.hpp"

#include <imgui/imgui.h>

void BaseWindow::setTitle(const std::string &title) {
	_actualTitle = title + "###" + _id;
}

void BaseWindow::setTitle(const std::string &title, bool useAsDefault) {
	if (useAsDefault)
		_defaultTitle = title;

	setTitle(title);
}

void BaseWindow::restoreDefaultTitle() {
	setTitle(_defaultTitle);
}

BaseWindow::BaseWindow(const std::string &id, int flags)
		: _id(id),
		  _flags(flags) {
	BaseWindow::setTitle(id, true);
}

BaseWindow::BaseWindow(const std::string &id, const std::string &title, int flags)
		: _id(id),
		  _flags(flags) {
	BaseWindow::setTitle(title.empty() ? id : title, true);
}

void BaseWindow::draw() {
	if (!_open)
		return;

	onBeginPre();

	bool collapsed = !ImGui::Begin(_actualTitle.c_str(), &_open, _flags);

	onBeginPost();

	if (!collapsed)
		onDraw();

	ImGui::End();
}

void BaseWindow::render() {
	if (!_open) // TODO: also check if window is collapsed
		return;

	onRender();
}

void BaseWindow::onBeginPre() {}

void BaseWindow::onBeginPost() {}

void BaseWindow::onDraw() {}

void BaseWindow::onRender() {}