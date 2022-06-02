#include "KnownGame.hpp"

bool KnownGame::known() const {
	return true;
}

const std::string &KnownGame::id() const {
	return _id;
}

const std::string &KnownGame::name() const {
	return _name;
}

std::shared_ptr<const LayerTemplate> KnownGame::findLayerTemplateById(const std::string &id) const {
	const auto it = _layerTemplates.find(id);

	if (it == _layerTemplates.end())
		return std::make_shared<LayerTemplate>(id);

	return it->second;
}

KnownGame::KnownGame(std::string id, std::string name)
		: _id(std::move(id)),
		  _name(std::move(name)) {}

void KnownGame::addLayerTemplate(const std::string &id, std::shared_ptr<LayerTemplate> layerTemplate) {
	_layerTemplates.insert({id, std::move(layerTemplate)});
}