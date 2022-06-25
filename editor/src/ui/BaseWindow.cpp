#include "BaseWindow.hpp"

#include <imgui/imgui.h>

const std::string &BaseWindow::getId() const {
	return _id;
}

const std::string &BaseWindow::getIdWithHashPrefix() const {
	return _idWithHashPrefix;
}

bool &BaseWindow::isOpen() {
	return _open;
}

void BaseWindow::setOpen(bool open) {
	_open = open;
}

void BaseWindow::setTitle(const std::string &title) {
	_actualTitle = title + _idWithHashPrefix;
}

void BaseWindow::setTitle(const std::string &title, bool useAsDefault) {
	if (useAsDefault)
		_defaultTitle = title;

	setTitle(title);
}

void BaseWindow::restoreDefaultTitle() {
	setTitle(_defaultTitle);
}

void BaseWindow::setPadding(glm::vec2 padding) {
	_padding = padding;
}

BaseWindow::BaseWindow(const std::string &id, int flags)
		: _id(id),
		  _idWithHashPrefix("###" + id),
		  _flags(flags) {
	BaseWindow::setTitle(id, true);
}

BaseWindow::BaseWindow(const std::string &id, const std::string &title, int flags)
		: _id(id),
		  _idWithHashPrefix("###" + id),
		  _flags(flags) {
	BaseWindow::setTitle(title.empty() ? id : title, true);
}

void BaseWindow::draw() {
	if (!_open)
		return;

	onBeginPre();

	{
		auto padding = _padding + 1.f;

		if (_previouslyDocked)
			padding.x -= 1;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, padding);
	}

	bool collapsed = !ImGui::Begin(_actualTitle.c_str(), &_open, _flags);

	ImGui::PopStyleVar();

	onBeginPost();

	if (!collapsed)
		onDraw();

	_previouslyDocked = ImGui::GetWindowDockID() != 0;

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