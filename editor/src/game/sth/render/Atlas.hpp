#pragma once

#include <cstdint>
#include <filesystem>
#include <unordered_map>

#include <GL/gl3w.h>
#include <glm/vec2.hpp>

#include "../../../core/Tilemap.hpp"

class Atlas {
public:
	class Entry {
	public:
		explicit Entry(uint16_t firstFrame, uint16_t frameCount = 1);

		uint16_t firstFrame() const;

		uint16_t frameCount() const;

	private:
		uint16_t _firstFrame;
		uint16_t _frameCount;
	};

	std::vector<glm::vec2> loadFromFile(const std::filesystem::path &path);

	const Entry &findEntryForTile(Tilemap::tile_t tile) const;

private:
	std::unordered_map<Tilemap::tile_t, Entry> _entries;
	Entry _defaultEntry{0};
};