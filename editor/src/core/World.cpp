#include "World.hpp"

#include "IWorldFactory.hpp"

namespace fs = std::filesystem;

World::World(std::shared_ptr<IGame> game, std::string name)
		: _game(std::move(game)),
		  _name(std::move(name)) {}

std::string &World::name() {
	return _name;
}

const std::string &World::name() const {
	return _name;
}

const fs::path &World::path() const {
	return _path;
}

void World::setPath(const fs::path &path) {
	_path = path;
	_filename = path.filename().u8string();
}

const std::string &World::filename() const {
	return _filename;
}

const std::string &World::nameOrFilename() const {
	if (_name.empty()) {
		if (_filename.empty()) {
			static const std::string emptyName = "(Unnamed)";
			return emptyName;
		}

		return _filename;
	}

	return name();
}

const std::vector<std::shared_ptr<Level>> &World::levels() const {
	return _levels;
}

void World::addLevel(const std::shared_ptr<Level> &level) {
	_levels.push_back(level);
}

std::shared_ptr<IGame> World::game() {
	return _game;
}

std::shared_ptr<const IGame> World::game() const {
	return _game;
}

std::shared_ptr<IWorldFactory> World::factory() const {
	return _factory;
}

void World::setFactory(std::shared_ptr<IWorldFactory> factory) {
	_factory = std::move(factory);
}

std::shared_ptr<Level> World::createLevel(std::shared_ptr<LevelSkin> skin, const std::string &name) {
	if (!(_factory && _game))
		return {};

	return _factory->createLevel(*_game, name, std::move(skin));
}