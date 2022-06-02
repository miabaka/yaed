#include "Tilemap.hpp"

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

Tilemap::tile_t Tilemap::operator()(int x, int y) const {
	if (!positionIsValid({x, y}))
		return 0;

	return _tiles[y * _size.x + x];
}