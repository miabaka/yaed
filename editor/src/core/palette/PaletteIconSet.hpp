#pragma once

#include <optional>
#include <unordered_map>

#include "../util/TextureSlice.hpp"
#include "../Tilemap.hpp"
#include "Brush.hpp"

class PaletteIconSet {
public:
	std::optional<TextureSlice> getIconForBrush(const Brush &brush) const;

private:
	std::unordered_map<Tilemap::tile_t, TextureSlice> _textureSlices;
};