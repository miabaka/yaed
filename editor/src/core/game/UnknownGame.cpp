#include "UnknownGame.hpp"

#include <fmt/format.h>

UnknownGame::UnknownGame(std::string id)
		: _id(std::move(id)),
		  _name(fmt::format("Unknown ({})", id)) {}

bool UnknownGame::known() const {
	return false;
}

const std::string &UnknownGame::id() const {
	return _id;
}

const std::string &UnknownGame::name() const {
	return _name;
}

std::shared_ptr<const PaletteTemplate> UnknownGame::paletteTemplate() const {
	return {};
}

std::shared_ptr<const LayerTemplate> UnknownGame::findLayerTemplateById(const std::string &id) const {
	return std::make_shared<LayerTemplate>(id);
}

std::shared_ptr<LevelSkin> UnknownGame::findLevelSkinById(int id) {
	return std::make_shared<LevelSkin>(id);
}

const std::map<int, std::shared_ptr<LevelSkin>> &UnknownGame::levelSkins() {
	static std::map<int, std::shared_ptr<LevelSkin>> skins;
	return skins;
}

std::shared_ptr<LevelSkin> UnknownGame::defaultLevelSkin() {
	return {};
}