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

BaseWindow::BaseWindow(const std::string &id, const std::string &title)
		: _id(id) {
	BaseWindow::setTitle(title.empty() ? id : title, true);
}

void BaseWindow::draw() {
	if (!_open)
		return;

	onBeginPre();

	bool collapsed = !ImGui::Begin(_actualTitle.c_str(), &_open);

	onBeginPost();

	if (!collapsed)
		onDraw();

	ImGui::End();
}

void BaseWindow::onBeginPre() {}

void BaseWindow::onBeginPost() {}

void BaseWindow::onDraw() {}