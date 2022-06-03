#include "PaletteTemplateBuilder.hpp"

PaletteTemplateBuilder &PaletteTemplateBuilder::beginGroup(const std::string &name) {
	_currentGroup = std::make_shared<BrushGroup>(name);
	return *this;
}

PaletteTemplateBuilder &PaletteTemplateBuilder::brush(const std::string &name, Brush::range_t range) {
	if (_currentGroup)
		_currentGroup->addBrush(std::make_shared<Brush>(name, range));

	return *this;
}

PaletteTemplateBuilder &PaletteTemplateBuilder::submitGroup() {
	if (_currentGroup) {
		_template.addBrushGroup(_currentGroup);
		_currentGroup.reset();
	}

	return *this;
}

std::shared_ptr<PaletteTemplate> PaletteTemplateBuilder::collect() {
	return std::make_shared<PaletteTemplate>(std::move(_template));
}