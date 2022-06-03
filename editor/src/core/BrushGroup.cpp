#include "BrushGroup.hpp"

BrushGroup::BrushGroup(std::string name)
		: _name(std::move(name)) {}

const std::string &BrushGroup::name() const {
	return _name;
}

void BrushGroup::addBrush(std::shared_ptr<Brush> brush) {

}