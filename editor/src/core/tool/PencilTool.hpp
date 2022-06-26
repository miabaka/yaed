#pragma once

#include <vector>

#include <glm/vec2.hpp>

#include "../palette/Brush.hpp"
#include "../Tilemap.hpp"
#include "ITool.hpp"

class PencilTool : public ITool {
public:
	const std::string &name() const override;

	void moveTo(glm::ivec2 position);

	void lineTo(glm::ivec2 position);

	void stroke(Tilemap &tilemap, const Brush &brush);

	void discard();

private:
	std::vector<glm::ivec2> _pathTiles;
	glm::ivec2 _currentPosition{};
};