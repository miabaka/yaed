#include "Brush.hpp"

Brush::Brush(std::string name, const range_t &range, bool unique)
		: _name(std::move(name)),
		  _range(range),
		  _unique(unique) {}

const std::string &Brush::name() const {
	return _name;
}

const Brush::range_t &Brush::range() const {
	return _range;
}

bool Brush::unique() const {
	return _unique;
}