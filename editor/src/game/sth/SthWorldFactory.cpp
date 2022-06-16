#include "SthWorldFactory.hpp"

const std::string &SthWorldFactory::id() const {
	static std::string id = "sth";
	return id;
}

const std::string &SthWorldFactory::name() const {
	static std::string name = "SthWorldFactory";
	return name;
}

std::shared_ptr<World> SthWorldFactory::createWorld(std::shared_ptr<IGame> game, std::string name) {
	if (game->id() != "sth")
		return {};

	return std::make_shared<World>(game, name);
}

std::shared_ptr<Level>
SthWorldFactory::createLevel(const IGame &game, std::string name, std::shared_ptr<LevelSkin> skin) {
	if (game.id() != "sth")
		return {};

	const glm::ivec2 size = {40, 30};

	auto mainLayer = std::make_shared<Layer>(game.findLayerTemplateById("main"), size);
	auto gemLayer = std::make_shared<Layer>(game.findLayerTemplateById("gems"), size);

	return std::make_shared<Level>(name, skin, std::vector{gemLayer, mainLayer});
}