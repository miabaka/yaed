#pragma once

#include <memory>
#include <unordered_set>

#include <glm/vec2.hpp>

#include "../core/Level.hpp"
#include "../tool/PencilTool.hpp"

class ToolHandler {
public:
	struct ToolUpdateParams {
		std::shared_ptr<Level> level;
		std::shared_ptr<BrushSelection> brushSelection;
		glm::vec2 tileSize;
	};

	void update(const ToolUpdateParams &params);

private:
	PencilTool _tool;
	std::unordered_set<ITool::MouseButton> _pressedButtons;
	glm::vec2 _previousLocalMousePos;
	glm::vec2 _previousLocalMouseTilePos;
	bool _previouslyHovered = false;
	bool _previousMousePosSet = false;
	bool _leaveEventTriggered = false;

	void handleMouseEvents(glm::vec2 tileSize);
};