#include "Brush.hpp"

Brush::Brush(std::string name, const range_t &range)
		: _name(std::move(name)),
		  _range(range) {}

const std::string &Brush::name() const {
	return _name;
}

const Brush::range_t &Brush::range() const {
	return _range;
}