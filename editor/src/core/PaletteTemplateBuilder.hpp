#pragma once

#include <memory>
#include <string>
#include "Brush.hpp"
#include "BrushGroup.hpp"
#include "PaletteTemplate.hpp"

class PaletteTemplateBuilder {
	friend class PaletteTemplate;

public:
	PaletteTemplateBuilder &beginGroup(const std::string &name);

	PaletteTemplateBuilder &brush(const std::string &name, Brush::range_t range);

	PaletteTemplateBuilder &submitGroup();

	std::shared_ptr<PaletteTemplate> collect();

private:
	PaletteTemplateBuilder() = default;

	std::shared_ptr<BrushGroup> _currentGroup;
	PaletteTemplate _template;
};