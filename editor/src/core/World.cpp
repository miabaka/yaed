#include "World.hpp"

World::World(std::string name)
		: _name(std::move(name)) {}

const std::string &World::name() const {
	return _name;
}

const std::vector<std::shared_ptr<Level>> &World::levels() const {
	return _levels;
}

void World::addLevel(const std::shared_ptr<Level> &level) {
	_levels.push_back(level);
}
