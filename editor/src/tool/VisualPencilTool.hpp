#pragma once

#include "../core/palette/BrushSelection.hpp"
#include "../core/tool/PencilTool.hpp"
#include "../core/Tilemap.hpp"
#include "BaseVisualTool.hpp"

class VisualPencilTool : public BaseVisualTool, public PencilTool {
public:
	void handleMouseEnter() override;

	void handleMouseMove(glm::ivec2 position, glm::ivec2 tilePosition) override;

	void handleMousePress(MouseButton button, glm::ivec2 position, glm::ivec2 tilePosition) override;

	void handleMouseRelease(MouseButton button, glm::ivec2 position, glm::ivec2 tilePosition) override;

	void update(Tilemap &tilemap, std::weak_ptr<BrushSelection> brushSelection);

private:
	bool _drawing = false;
	bool _mouseJustEntered = false;
	MouseButton _drawingButton{};
	std::shared_ptr<Brush> _drawingBrush;
	std::weak_ptr<BrushSelection> _brushSelection;
};