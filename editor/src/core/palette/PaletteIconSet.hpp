#pragma once

#include <optional>
#include <unordered_map>

#include "../util/TextureSlice.hpp"
#include "../Tilemap.hpp"
#include "Brush.hpp"

class PaletteIconSet {
public:
	const TextureSlice &getIconForBrush(const Brush &brush) const;

	void setIcon(Tilemap::tile_t tile, TextureSlice texture);

private:
	std::unordered_map<Tilemap::tile_t, TextureSlice> _textureSlices;
};