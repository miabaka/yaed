#pragma once

#include <string>
#include <glm/vec2.hpp>

class ITool {
public:
	enum class MouseButton {
		Left,
		Middle,
		Right
	};

	virtual ~ITool() = default;

	virtual const std::string &name() const = 0;

	virtual void handleMouseEnter() = 0;

	virtual void handleMouseLeave() = 0;

	virtual void handleMouseMove(glm::ivec2 position, glm::ivec2 tilePosition) = 0;

	virtual void handleMousePress(MouseButton button, glm::ivec2 position, glm::ivec2 tilePosition) = 0;

	virtual void handleMouseRelease(MouseButton button, glm::ivec2 position, glm::ivec2 tilePosition) = 0;
};