#pragma once

#include <glm/vec2.hpp>

class IVisualTool {
public:
	enum class MouseButton {
		Left,
		Middle,
		Right
	};

	virtual ~IVisualTool() = default;

	virtual void handleMouseEnter() = 0;

	virtual void handleMouseLeave() = 0;

	virtual void handleMouseMove(glm::ivec2 position, glm::ivec2 tilePosition) = 0;

	virtual void handleMousePress(MouseButton button, glm::ivec2 position, glm::ivec2 tilePosition) = 0;

	virtual void handleMouseRelease(MouseButton button, glm::ivec2 position, glm::ivec2 tilePosition) = 0;
};