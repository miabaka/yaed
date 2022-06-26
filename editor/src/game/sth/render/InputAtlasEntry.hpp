#pragma once

#include <vector>
#include <unordered_set>

#include <glm/vec2.hpp>

#include "../../../core/Tilemap.hpp"

struct InputAtlasEntry {
	std::unordered_set<Tilemap::tile_t> tiles;
	glm::ivec2 position;
	glm::ivec2 frameSize;
	int frameCount;
	int framesPerLine;

	void computeOffsets(std::vector<glm::vec2> &out, glm::ivec2 atlasSize) const;
};