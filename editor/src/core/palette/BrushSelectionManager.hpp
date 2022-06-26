#pragma once

#include <memory>
#include <unordered_map>

#include "../LayerTemplate.hpp"
#include "../Level.hpp"
#include "BrushSelection.hpp"
#include "PaletteTemplate.hpp"

// TODO: automatically clean unused old selection pairs
//       actually maybe not really needed
class BrushSelectionManager {
public:
	std::shared_ptr<BrushSelection> getSelectionForLevel(const Level &level, const PaletteTemplate &paletteTemplate);

	std::shared_ptr<BrushSelection> getSelectionForLevel(const Level &level) const;

private:
	std::unordered_map<std::shared_ptr<const LayerTemplate>, std::shared_ptr<BrushSelection>> _selection;
};