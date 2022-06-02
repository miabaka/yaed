#pragma once

#include <string>
#include <set>

#include "Tilemap.hpp"

class LayerTemplate {
public:
	explicit LayerTemplate(std::string layerName, std::set<Tilemap::tile_t> allowedTiles = {});

	const std::string &layerName() const;

	const std::set<Tilemap::tile_t> &allowedTiles() const;

private:
	std::string _layerName;
	std::set<Tilemap::tile_t> _allowedTiles;
};