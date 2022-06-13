#include "LayerTemplate.hpp"

LayerTemplate::LayerTemplate(std::string layerName, std::unordered_set<Tilemap::tile_t> allowedTiles)
		: _layerName(std::move(layerName)),
		  _allowedTiles(std::move(allowedTiles)) {}

const std::string &LayerTemplate::layerName() const {
	return _layerName;
}

const std::unordered_set<Tilemap::tile_t> &LayerTemplate::allowedTiles() const {
	return _allowedTiles;
}