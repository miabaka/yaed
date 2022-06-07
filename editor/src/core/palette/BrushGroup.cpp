#include "BrushGroup.hpp"

BrushGroup::BrushGroup(std::string name)
		: _name(std::move(name)) {}

const std::string &BrushGroup::name() const {
	return _name;
}

const std::vector<std::shared_ptr<Brush>> &BrushGroup::brushes() const {
	return _brushes;
}

void BrushGroup::addBrush(std::shared_ptr<Brush> brush) {
	_brushes.emplace_back(std::move(brush));
}