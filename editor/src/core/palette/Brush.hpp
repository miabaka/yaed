#pragma once

#include <string>
#include "../Tilemap.hpp"

class Brush {
public:
	using range_t = Tilemap::tile_range_t;

	Brush(std::string name, const range_t &range, bool unique = false);

	const std::string &name() const;

	const range_t &range() const;

	bool unique() const;

private:
	std::string _name;
	range_t _range;
	bool _unique;
};