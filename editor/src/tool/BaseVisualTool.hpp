#pragma once

#include "IVisualTool.hpp"

class BaseVisualTool : public IVisualTool {
public:
	void handleMouseEnter() override;

	void handleMouseLeave() override;

	void handleMouseMove(glm::ivec2 position, glm::ivec2 tilePosition) override;

	void handleMousePress(MouseButton button, glm::ivec2 position, glm::ivec2 tilePosition) override;

	void handleMouseRelease(MouseButton button, glm::ivec2 position, glm::ivec2 tilePosition) override;
};