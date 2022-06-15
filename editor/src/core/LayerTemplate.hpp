#pragma once

#include <string>
#include <unordered_set>

#include "Tilemap.hpp"

class LayerTemplate {
public:
	explicit LayerTemplate(std::string layerName, std::unordered_set<Tilemap::tile_t> allowedTiles = {});

	const std::string &layerName() const;

	const std::unordered_set<Tilemap::tile_t> &allowedTiles() const;

	Tilemap::tile_t defaultPrimaryTile() const;

	void setDefaultPrimaryTile(Tilemap::tile_t tile);

	Tilemap::tile_t defaultSecondaryTile() const;

	void setDefaultSecondaryTile(Tilemap::tile_t tile);

private:
	std::string _layerName;
	std::unordered_set<Tilemap::tile_t> _allowedTiles;
	Tilemap::tile_t _defaultPrimaryTile = 0;
	Tilemap::tile_t _defaultSecondaryTile = 0;
};