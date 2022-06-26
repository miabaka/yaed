#include "PaletteIconSet.hpp"

const TextureSlice &PaletteIconSet::getIconForBrush(const Brush &brush) const {
	const auto it = _textureSlices.find(brush.range().start());

	if (it == _textureSlices.end()) {
		static TextureSlice stub;
		return stub;
	}

	return it->second;
}

void PaletteIconSet::setIcon(Tilemap::tile_t tile, TextureSlice texture) {
	_textureSlices[tile] = texture;
}