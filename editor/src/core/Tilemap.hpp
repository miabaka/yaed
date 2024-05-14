#pragma once

#include <memory>
#include <span>
#include <stdexcept>
#include <vector>

#include <glm/vec2.hpp>

#include "util/Rect.hpp"
#include "TilemapDefs.hpp"
#include "UniqueTileAllocator.hpp"

class Tilemap {
public:
	using tile_t = TilemapDefs::tile_t;
	using tile_range_t = TilemapDefs::tile_range_t;

	explicit Tilemap(glm::ivec2 size, tile_t value = 0);

	~Tilemap() = default;

	[[nodiscard]]
	static bool isSizeValid(glm::ivec2 size);

	[[nodiscard]]
	bool isPositionValid(glm::ivec2 position) const;

	[[nodiscard]]
	size_t tileCount() const;

	[[nodiscard]]
	glm::ivec2 size() const;

	[[nodiscard]]
	std::span<tile_t> tiles();

	[[nodiscard]]
	std::span<const tile_t> tiles() const;

	[[nodiscard]]
	tile_t operator()(glm::ivec2 position) const;

	[[nodiscard]]
	tile_t get(glm::ivec2 position, glm::ivec2 offset, tile_t defaultTile) const;

	[[nodiscard]]
	tile_t getOccupied(glm::ivec2 position, glm::ivec2 offset, tile_t defaultTile) const;

	bool set(glm::ivec2 position, tile_t tile);

	void registerUniqueTileRange(tile_range_t range);

	/**
	 * Must be called after raw (not using set method) tilemap changes
	 */
	void processRawChanges();

	void setClipRect(IntRect rect);

	[[nodiscard]]
	IntRect occupiedRegion() const;

	void setMinimalOccupiedRegionSize(glm::ivec2 size);

private:
	UniqueTileAllocator _uniqueTiles;
	glm::ivec2 _size;
	glm::ivec2 _minOccupiedRegionSize{};
	IntRect _clipRect;
	mutable IntRect _occupiedRegion;
	mutable bool _occupiedRegionDirty = true;
	std::vector<tile_t> _tiles{};

	void invalidateOccupiedRegion();

	void updateOccupiedRegion() const;
};