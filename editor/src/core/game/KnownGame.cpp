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

std::shared_ptr<const PaletteTemplate> KnownGame::paletteTemplate() const {
	return _paletteTemplate;
}

std::shared_ptr<const LayerTemplate> KnownGame::findLayerTemplateById(const std::string &id) const {
	const auto it = _layerTemplates.find(id);

	if (it == _layerTemplates.end())
		return std::make_shared<LayerTemplate>(id);

	return it->second;
}

std::shared_ptr<LevelSkin> KnownGame::findLevelSkinById(int id) {
	const auto it = _levelSkins.find(id);

	if (it == _levelSkins.end())
		return std::make_shared<LevelSkin>(id);

	return it->second;
}

KnownGame::KnownGame(std::string id, std::string name)
		: _id(std::move(id)),
		  _name(std::move(name)) {}

void KnownGame::addLayerTemplate(const std::string &id, std::shared_ptr<LayerTemplate> layerTemplate) {
	_layerTemplates.insert({id, std::move(layerTemplate)});
}

void KnownGame::addLevelSkin(std::shared_ptr<LevelSkin> levelSkin) {
	_levelSkins.insert({levelSkin->id(), std::move(levelSkin)});
}

void KnownGame::setPaletteTemplate(std::shared_ptr<PaletteTemplate> paletteTemplate) {
	_paletteTemplate = std::move(paletteTemplate);
}

const std::map<int, std::shared_ptr<LevelSkin>> &KnownGame::levelSkins() {
	return _levelSkins;
}