#include "Level.hpp"

Level::Level(std::string name, int skinIndex, std::vector<std::shared_ptr<Layer>> layers)
		: _name(std::move(name)),
		  _skinIndex(skinIndex),
		  _layers(std::move(layers)) {}

int Level::skinIndex() const {
	return _skinIndex;
}

const std::string &Level::name() const {
	return _name;
}

const std::vector<std::shared_ptr<Layer>> &Level::layers() const {
	return _layers;
}