#include "LayerTemplate.hpp"

LayerTemplate::LayerTemplate(std::string layerName)
		: _layerName(std::move(layerName)) {}

const std::string &LayerTemplate::layerName() const {
	return _layerName;
}
