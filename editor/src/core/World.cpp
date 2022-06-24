#include "World.hpp"

#include "export/IWorldExporter.hpp"
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

std::shared_ptr<Level> World::createLevel(std::shared_ptr<LevelSkin> skin, std::string name) {
	if (!(_factory && _game))
		return {};

	if (!skin)
		skin = _game->defaultLevelSkin();

	if (!skin)
		return {};

	if (name.empty())
		name = std::to_string(_levels.size() + 1);

	std::shared_ptr<Level> level = _factory->createLevel(*_game, name, std::move(skin));

	if (level)
		_levels.push_back(level);

	return std::move(level);
}

std::shared_ptr<IWorldExporter> World::exporter() const {
	return _exporter;
}

void World::setExporter(std::shared_ptr<IWorldExporter> exporter) {
	_exporter = std::move(exporter);
}

bool World::removeMarkedLevels() {
	bool somethingRemoved = false;

	for (ptrdiff_t nLevel = 0; nLevel < _levels.size(); nLevel++) {
		if (!_levels[nLevel]->markedForRemoval())
			continue;

		_levels.erase(_levels.begin() + nLevel);
		somethingRemoved = true;

		nLevel--;
	}

	return somethingRemoved;
}