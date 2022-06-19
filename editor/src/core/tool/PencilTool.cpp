#include "PencilTool.hpp"

#include <glm/common.hpp>

const std::string &PencilTool::name() const {
	static std::string name = "Pencil";
	return name;
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