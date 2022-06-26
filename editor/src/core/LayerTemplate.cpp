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

Tilemap::tile_t LayerTemplate::defaultPrimaryTile() const {
	return _defaultPrimaryTile;
}

void LayerTemplate::setDefaultPrimaryTile(Tilemap::tile_t tile) {
	if (_allowedTiles.find(tile) == _allowedTiles.end())
		return;

	_defaultPrimaryTile = tile;
}

Tilemap::tile_t LayerTemplate::defaultSecondaryTile() const {
	return _defaultSecondaryTile;
}

void LayerTemplate::setDefaultSecondaryTile(Tilemap::tile_t tile) {
	if (_allowedTiles.find(tile) == _allowedTiles.end())
		return;

	_defaultSecondaryTile = tile;
}