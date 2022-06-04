#include "TreasureHunterWorldFactory.hpp"

const std::string &TreasureHunterWorldFactory::id() const {
	static std::string id = "sth";
	return id;
}

const std::string &TreasureHunterWorldFactory::name() const {
	static std::string name = "TreasureHunterWorldFactory";
	return name;
}

std::shared_ptr<World> TreasureHunterWorldFactory::createWorld(const GameManager &gameManager, std::string name) {
	std::shared_ptr<BaseGame> game = gameManager.findGameById("sth");
	return std::make_shared<World>(game, name);
}

std::shared_ptr<Level> TreasureHunterWorldFactory::createLevel(
		const GameManager &gameManager, std::string name, std::shared_ptr<LevelSkin> skin) {
	std::shared_ptr<BaseGame> game = gameManager.findGameById("sth");

	glm::ivec2 size = {40, 30};

	auto mainLayer = std::make_shared<Layer>(game->findLayerTemplateById("main"), size);
	auto gemLayer = std::make_shared<Layer>(game->findLayerTemplateById("gems"), size);

	return std::make_shared<Level>(name, skin, std::vector{mainLayer, gemLayer});
}