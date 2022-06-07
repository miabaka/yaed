#include "World.hpp"

World::World(std::shared_ptr<IGame> game, std::string name)
		: _game(std::move(game)),
		  _name(std::move(name)) {}

std::string &World::name() {
	return _name;
}

const std::string &World::name() const {
	return _name;
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