#include "PaletteTemplate.hpp"

#include "PaletteTemplateBuilder.hpp"

PaletteTemplateBuilder PaletteTemplate::builder() {
	return {};
}

void PaletteTemplate::addBrushGroup(std::shared_ptr<BrushGroup> group) {
	_brushGroups.emplace_back(std::move(group));
}

const std::vector<std::shared_ptr<BrushGroup>> &PaletteTemplate::brushGroups() const {
	return _brushGroups;
}

std::shared_ptr<Brush> PaletteTemplate::findBrushForTile(Tilemap::tile_t tile) const {
	for (const std::shared_ptr<BrushGroup> &group: _brushGroups) {
		for (std::shared_ptr<Brush> brush: group->brushes()) {
			if (brush->range().inclusiveContains(tile))
				return brush;
		}
	}

	return {};
}