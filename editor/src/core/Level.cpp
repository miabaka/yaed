#include "Level.hpp"

Level::Level(std::string name, std::shared_ptr<LevelSkin> skin, std::vector<std::shared_ptr<Layer>> layers)
		: _name(std::move(name)),
		  _skin(std::move(skin)),
		  _layers(std::move(layers)) {
	selectLayer(_layers.back());
}

std::string &Level::name() {
	return _name;
}

const std::string &Level::name() const {
	return _name;
}

std::shared_ptr<LevelSkin> Level::skin() const {
	return _skin;
}

void Level::setSkin(std::shared_ptr<LevelSkin> skin) {
	_skin = std::move(skin);
}

std::shared_ptr<Layer> Level::selectedLayer() const {
	return _selectedLayer.lock();
}

void Level::selectLayer(std::shared_ptr<Layer> layer) {
	_selectedLayer = layer;
}

const std::vector<std::shared_ptr<Layer>> &Level::layers() const {
	return _layers;
}

std::shared_ptr<World> Level::world() {
	return _world.lock();
}

void Level::setWorld(std::shared_ptr<World> world) {
	_world = world;
}