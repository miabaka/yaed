#include "PencilTool.hpp"

#include <glm/common.hpp>

const std::string &PencilTool::name() const {
	static std::string name = "Pencil";
	return name;
}

void PencilTool::handleMouseEnter() {
	_mouseJustEntered = true;
}

void PencilTool::handleMouseMove(glm::ivec2 position, glm::ivec2 tilePosition) {
	if (!_drawing)
		return;

	if (_mouseJustEntered) {
		moveTo(tilePosition);
		_mouseJustEntered = false;
	} else {
		lineTo(tilePosition);
	}
}

void PencilTool::handleMousePress(MouseButton button, glm::ivec2 position, glm::ivec2 tilePosition) {
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

void PencilTool::handleMouseRelease(MouseButton button, glm::ivec2 position, glm::ivec2 tilePosition) {
	if (button != _drawingButton)
		return;

	_drawing = false;
	_drawingBrush = {};
}

void PencilTool::update(Tilemap &tilemap, std::weak_ptr<BrushSelection> brushSelection) {
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

void PencilTool::moveTo(glm::ivec2 position) {
	_currentPosition = position;
}

// based on https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm#All_cases
void PencilTool::lineTo(glm::ivec2 position) {
	auto pointA = _currentPosition, pointB = position;

	auto delta = glm::abs(pointB - pointA) * glm::ivec2{1, -1};
	auto slope = glm::sign(pointB - pointA);

	if (slope.x == 0) slope.x = 1;
	if (slope.y == 0) slope.y = 1;

	auto error = delta.x + delta.y;

	do {
		_pathTiles.push_back(pointA);

		auto e2 = error * 2;

		if (e2 >= delta.y) {
			if (pointA.x == pointB.x)
				break;

			error += delta.y;
			pointA.x += slope.x;
		}

		if (e2 <= delta.x) {
			if (pointA.y == pointB.y)
				break;

			error += delta.x;
			pointA.y += slope.y;
		}
	} while (pointA != pointB);

	_currentPosition = position;
}

void PencilTool::stroke(Tilemap &tilemap, const Brush &brush) {
	for (auto position: _pathTiles)
		tilemap.set(position, brush.range().start());

	discard();
}

void PencilTool::discard() {
	_pathTiles.clear();
}