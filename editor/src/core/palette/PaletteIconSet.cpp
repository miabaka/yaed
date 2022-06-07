#include "PaletteIconSet.hpp"

std::optional<TextureSlice> PaletteIconSet::getIconForBrush(const Brush &brush) const {
	const auto it = _textureSlices.find(brush.range().start());

	if (it == _textureSlices.end())
		return {};

	return it->second;
}