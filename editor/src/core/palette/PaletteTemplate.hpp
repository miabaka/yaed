#pragma once

#include <memory>
#include <vector>
#include "BrushGroup.hpp"

class PaletteTemplateBuilder;

class PaletteTemplate {
public:
	static PaletteTemplateBuilder builder();

	void addBrushGroup(std::shared_ptr<BrushGroup> group);

	const std::vector<std::shared_ptr<BrushGroup>> &brushGroups() const;

private:
	std::vector<std::shared_ptr<BrushGroup>> _brushGroups;
};