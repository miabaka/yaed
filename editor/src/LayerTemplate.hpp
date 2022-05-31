#pragma once

#include <string>
#include <unordered_set>

#include "Tilemap.hpp"

class LayerTemplate {
public:
	explicit LayerTemplate(std::string layerName);

	const std::string &layerName() const;

private:
	std::string _layerName;
	std::unordered_set<Tilemap::tile_t> _allowedTiles;
};