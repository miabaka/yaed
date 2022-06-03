#pragma once

#include <string>

#include "util/Range.hpp"
#include "Tilemap.hpp"

class Brush {
public:
	typedef Range<Tilemap::tile_t> range_t;

	explicit Brush(std::string name, const range_t &range);

	const std::string &name() const;

	const range_t &range() const;

private:
	std::string _name;
	range_t _range;
};