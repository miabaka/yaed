#pragma once

#include <memory>
#include <vector>
#include "BrushGroup.hpp"

class PaletteTemplateBuilder;

class PaletteTemplate {
public:
	void addBrushGroup(std::shared_ptr<BrushGroup> group);

	static PaletteTemplateBuilder builder();

private:
	std::vector<BrushGroup> _brushGroups;
};