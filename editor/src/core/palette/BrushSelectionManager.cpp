#include "BrushSelectionManager.hpp"

std::shared_ptr<BrushSelection> BrushSelectionManager::getSelectionForLevel(
		const Level &level, const PaletteTemplate &paletteTemplate) {
	std::shared_ptr<Layer> layer = level.selectedLayer();

	if (!layer)
		return {};

	std::shared_ptr<const LayerTemplate> layerTemplate = layer->getTemplate();

	if (!layerTemplate)
		return {};

	std::shared_ptr<BrushSelection> &selection = _selection[layerTemplate];

	if (!selection) {
		selection = std::make_shared<BrushSelection>();

		selection->setPrimary(paletteTemplate.findBrushForTile(layerTemplate->defaultPrimaryTile()));
		selection->setSecondary(paletteTemplate.findBrushForTile(layerTemplate->defaultSecondaryTile()));
	}

	return selection;
}