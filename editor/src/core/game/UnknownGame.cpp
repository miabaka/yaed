#include "UnknownGame.hpp"

UnknownGame::UnknownGame(std::string id)
		: _id(std::move(id)) {}

bool UnknownGame::known() const {
	return false;
}

const std::string &UnknownGame::id() const {
	return _id;
}

const std::string &UnknownGame::name() const {
	static std::string name = "Unknown";
	return name;
}

std::shared_ptr<const LayerTemplate> UnknownGame::findLayerTemplateById(const std::string &id) const {
	return std::make_shared<LayerTemplate>(id);
}