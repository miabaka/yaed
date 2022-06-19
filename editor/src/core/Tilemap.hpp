#pragma once

#include <memory>
#include <stdexcept>
#include <vector>
#include <unordered_map>

#include <glm/vec2.hpp>

#include "TilemapDefs.hpp"
#include "UniqueTileAllocator.hpp"

class Tilemap {
public:
	using tile_t = TilemapDefs::tile_t;
	using tile_range_t = TilemapDefs::tile_range_t;

	explicit Tilemap(glm::ivec2 size, tile_t value = 0);

	Tilemap(Tilemap &) = delete;

	Tilemap(Tilemap &&) = default;

	~Tilemap() = default;

	static bool sizeIsValid(glm::ivec2 size);

	bool positionIsValid(glm::ivec2 position) const;

	size_t tileCount() const;

	glm::ivec2 size() const;

	tile_t *data() noexcept;

	const tile_t *data() const noexcept;

	Tilemap::tile_t operator()(glm::ivec2 position) const;

	bool set(glm::ivec2 position, Tilemap::tile_t tile);

	void registerUniqueTileRange(tile_range_t range);

	/**
	 * Must be called after raw (not using set method) tilemap changes
	 */
	void processRawChanges();

private:
	UniqueTileAllocator _uniqueTiles;
	glm::ivec2 _size;
	std::vector<tile_t> _tiles{};
};