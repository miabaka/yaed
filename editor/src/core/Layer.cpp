#include "Layer.hpp"

Layer::Layer(std::shared_ptr<const LayerTemplate> aTemplate, glm::ivec2 size)
		: _template(std::move(aTemplate)),
		  _tilemap(size) {}

Tilemap &Layer::tilemap() {
	return _tilemap;
}

const Tilemap &Layer::tilemap() const {
	return _tilemap;
}

const std::string &Layer::name() const {
	return _template->layerName();
}

std::shared_ptr<const LayerTemplate> Layer::getTemplate() const {
	return _template;
}