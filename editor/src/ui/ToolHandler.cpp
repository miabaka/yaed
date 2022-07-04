#include "ToolHandler.hpp"

#include <imgui/imgui.h>

void ToolHandler::update(const ToolUpdateParams &params) {
	handleMouseEvents(params.tileSize);

	if (!params.level)
		return;

	std::shared_ptr<Layer> layer = params.level->selectedLayer();

	if (!layer)
		return;

	_tool.update(layer->tilemap(), params.brushSelection);
}

void ToolHandler::handleMouseEvents(glm::vec2 tileSize) {
	const bool viewportHovered = ImGui::IsItemHovered();

	if (viewportHovered != _previouslyHovered) {
		_previouslyHovered = viewportHovered;

		if (viewportHovered) {
			_tool.handleMouseEnter();
			_leaveEventTriggered = false;

			if (_previousMousePosSet)
				_tool.handleMouseMove(_previousLocalMousePos, _previousLocalMouseTilePos);
		}
	}

	const glm::vec2 mousePos = ImGui::GetMousePos();

	if (std::max(std::abs(mousePos.x), std::abs(mousePos.y)) > 32768)
		return;

	const glm::vec2 viewportPos = ImGui::GetCursorScreenPos();
	const glm::vec2 localMousePos = mousePos - viewportPos;
	const glm::vec2 localMouseTilePos = localMousePos / tileSize;

	if (!_leaveEventTriggered && _previousMousePosSet && localMousePos != _previousLocalMousePos)
		_tool.handleMouseMove(localMousePos, localMouseTilePos);

	_previousLocalMousePos = localMousePos;
	_previousLocalMouseTilePos = localMouseTilePos;
	_previousMousePosSet = true;

	if (!viewportHovered && !_leaveEventTriggered) {
		_tool.handleMouseLeave();
		_leaveEventTriggered = true;
	}

	static const std::pair<ImGuiMouseButton, ITool::MouseButton> buttons[] = {
			{ImGuiMouseButton_Left,   ITool::MouseButton::Left},
			{ImGuiMouseButton_Middle, ITool::MouseButton::Middle},
			{ImGuiMouseButton_Right,  ITool::MouseButton::Right},
	};

	for (auto[imGuiButton, toolButton]: buttons) {
		const bool pressed = _pressedButtons.find(toolButton) != _pressedButtons.end();

		if (viewportHovered && ImGui::IsMouseClicked(imGuiButton)) {
			_tool.handleMousePress(toolButton, localMousePos, localMouseTilePos);
			_pressedButtons.insert(toolButton);
		} else if (pressed && ImGui::IsMouseReleased(imGuiButton)) {
			_tool.handleMouseRelease(toolButton, localMousePos, localMouseTilePos);
			_pressedButtons.erase(toolButton);
		}
	}
}