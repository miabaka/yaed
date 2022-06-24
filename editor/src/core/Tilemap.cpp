#include "Tilemap.hpp"

#include <glm/common.hpp>

Tilemap::Tilemap(glm::ivec2 size, tile_t value)
		: _size(size) {
	if (!sizeIsValid(size))
		throw std::runtime_error("Invalid size");

	_tiles.resize(size.x * size.y, value);
}

bool Tilemap::sizeIsValid(glm::ivec2 size) {
	return size.x > 0 && size.x <= 512 && size.y > 0 && size.y <= 512;
};

bool Tilemap::positionIsValid(glm::ivec2 position) const {
	return position.x >= 0 && position.x < _size.x && position.y >= 0 && position.y < _size.y;
}

size_t Tilemap::tileCount() const {
	return _size.x * _size.y;
}

glm::ivec2 Tilemap::size() const {
	return _size;
}

Tilemap::tile_t *Tilemap::data() noexcept {
	return _tiles.data();
};

const Tilemap::tile_t *Tilemap::data() const noexcept {
	return _tiles.data();
}

Tilemap::tile_t Tilemap::operator()(glm::ivec2 position) const {
	if (!positionIsValid(position))
		return 0;

	return _tiles[position.y * _size.x + position.x];
}

Tilemap::tile_t Tilemap::get(glm::ivec2 position, glm::ivec2 offset, tile_t defaultTile) const {
	position += offset;

	if (!positionIsValid(position))
		return defaultTile;

	return _tiles[position.y * _size.x + position.x];
}

Tilemap::tile_t Tilemap::getOccupied(glm::ivec2 position, glm::ivec2 offset, tile_t defaultTile) const {
	position += offset;

	if (!(positionIsValid(position) && _occupiedRegion.containsPoint(position)))
		return defaultTile;

	return _tiles[position.y * _size.x + position.x];
}

bool Tilemap::set(glm::ivec2 position, Tilemap::tile_t newTile) {
	if (!positionIsValid(position))
		return false;

	if (!(_clipRect.empty() || _clipRect.containsPoint(position)))
		return false;

	tile_t &tile = _tiles[position.y * _size.x + position.x];

	std::optional<tile_t> allocatedTile = _uniqueTiles.alloc(tile, newTile);

	if (!allocatedTile)
		return false;

	_uniqueTiles.free(tile);
	tile = *allocatedTile;

	// TODO: optimize based on tile change
	computeOccupiedRegion();

	return true;
}

void Tilemap::registerUniqueTileRange(tile_range_t range) {
	_uniqueTiles.registerUniqueRange(range);
}

void Tilemap::processRawChanges() {
	_uniqueTiles.resetUsage();

	for (const tile_t tile: _tiles)
		_uniqueTiles.incrementCounterFor(tile);

	computeOccupiedRegion();
}

void Tilemap::setClipRect(IntRect rect) {
	_clipRect = rect;
}

IntRect Tilemap::occupiedRegion() const {
	return _occupiedRegion;
}

void Tilemap::computeOccupiedRegion() {
	glm::ivec2 lowerBound = _size;
	glm::ivec2 upperBound{};

	for (int i = 0; i < _tiles.size(); i++) {
		const tile_t tile = _tiles[i];

		if (tile == 0)
			continue;

		const glm::ivec2 pos = {i % _size.x, i / _size.x};

		lowerBound = glm::min(lowerBound, pos);
		upperBound = glm::max(upperBound, pos);
	}

	if (lowerBound.x > upperBound.x || lowerBound.y > upperBound.y) {
		_occupiedRegion = {};
		return;
	}

	upperBound += 1;

	const glm::ivec2 regionSize = upperBound - lowerBound;
	const glm::ivec2 restMinSize = glm::max({}, _minOccupiedRegionSize - regionSize);
	const glm::ivec2 halfRestMinSize = restMinSize / 2;

	lowerBound -= restMinSize - halfRestMinSize;
	upperBound += halfRestMinSize;

	if (lowerBound.x < 0) {
		lowerBound.x = 0;
		upperBound.x = _minOccupiedRegionSize.x;
	}

	if (lowerBound.y < 0) {
		lowerBound.y = 0;
		upperBound.y = _minOccupiedRegionSize.y;
	}

	if (upperBound.x > _size.x) {
		lowerBound.x = _size.x - _minOccupiedRegionSize.x;
		upperBound.x = _size.x;
	}

	if (upperBound.y > _size.y) {
		lowerBound.y = _size.y - _minOccupiedRegionSize.y;
		upperBound.y = _size.y;
	}

	_occupiedRegion = {lowerBound, upperBound};
}

void Tilemap::setMinimalOccupiedRegionSize(glm::ivec2 size) {
	_minOccupiedRegionSize = size;
}