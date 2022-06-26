#include "VisualPencilTool.hpp"

void VisualPencilTool::handleMouseEnter() {
	_mouseJustEntered = true;
}

void VisualPencilTool::handleMouseMove(glm::ivec2 position, glm::ivec2 tilePosition) {
	if (!_drawing)
		return;

	if (_mouseJustEntered) {
		moveTo(tilePosition);
		_mouseJustEntered = false;
	} else {
		lineTo(tilePosition);
	}
}

void VisualPencilTool::handleMousePress(MouseButton button, glm::ivec2 position, glm::ivec2 tilePosition) {
	const bool mouseButtonAllowed =
			(button == MouseButton::Left || button == MouseButton::Right);

	if (_drawing || _brushSelection.expired() || !mouseButtonAllowed)
		return;

	_drawing = true;
	_drawingButton = button;

	std::shared_ptr<BrushSelection> brushSelection = _brushSelection.lock();

	if (button == MouseButton::Left)
		_drawingBrush = brushSelection->primary();
	else
		_drawingBrush = brushSelection->secondary();

	discard();

	moveTo(tilePosition);
	lineTo(tilePosition);
}

void VisualPencilTool::handleMouseRelease(MouseButton button, glm::ivec2 position, glm::ivec2 tilePosition) {
	if (button != _drawingButton)
		return;

	_drawing = false;
	_drawingBrush = {};
}

void VisualPencilTool::update(Tilemap &tilemap, std::weak_ptr<BrushSelection> brushSelection) {
	if (!_drawing) {
		_brushSelection = std::move(brushSelection);
		return;
	}

	if (!_drawingBrush) {
		discard();
		return;
	}

	stroke(tilemap, *_drawingBrush);
}