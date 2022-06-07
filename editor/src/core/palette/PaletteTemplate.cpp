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