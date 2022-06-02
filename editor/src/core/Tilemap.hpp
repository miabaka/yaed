#pragma once

#include <cstdint>
#include <stdexcept>
#include <vector>

#include <glm/vec2.hpp>

class Tilemap {
public:
	typedef uint16_t tile_t;

	explicit Tilemap(glm::ivec2 size, tile_t value = 0);

	Tilemap(Tilemap &) = default;

	Tilemap(Tilemap &&) = default;

	~Tilemap() = default;

	static bool sizeIsValid(glm::ivec2 size);

	bool positionIsValid(glm::ivec2 position) const;

	size_t tileCount() const;

	glm::ivec2 size() const;

	tile_t *data() noexcept;
  const tile_t *data() const noexcept;

	Tilemap::tile_t operator()(int x, int y) const;

private:
	std::vector<tile_t> _tiles{};
	glm::ivec2 _size;
};