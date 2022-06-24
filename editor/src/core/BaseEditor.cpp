#include "BaseEditor.hpp"

#include <stdexcept>
#include <algorithm>

namespace fs = std::filesystem;

struct EditorWorldData : public ICustomData {
	std::shared_ptr<IWorldExporter> exporter;
};

BaseEditor::BaseEditor()
		: _brushSelection(std::make_shared<BrushSelectionManager>()) {}

GameManager &BaseEditor::games() {
	return _gameManager;
}

WorldFormatManager &BaseEditor::worldFormats() {
	return _worldFormatManager;
}

WorldFactoryManager &BaseEditor::worldFactories() {
	return _worldFactoryManager;
}

PaletteIconProviderManager &BaseEditor::paletteIconProviders() {
	return _iconProviderManager;
}

TilemapRendererManager &BaseEditor::tilemapRenderers() {
	return _tilemapRendererManager;
}

std::shared_ptr<BrushSelectionManager> BaseEditor::brushSelection() {
	return _brushSelection;
}

const std::vector<std::shared_ptr<World>> &BaseEditor::worlds() const {
	return _worlds;
}

std::shared_ptr<World>
BaseEditor::createWorld(std::shared_ptr<IGame> game, std::shared_ptr<IWorldFactory> factory, const std::string &name) {
	if (!(game && factory))
		return {};

	std::shared_ptr<World> world = factory->createWorld(game, name);

	if (!world)
		return {};

	world->setFactory(std::move(factory));

	_worlds.push_back(world);
	selectWorld(world);

	return world;
}

std::shared_ptr<World> BaseEditor::openWorld(const std::filesystem::path &path) {
	std::shared_ptr<IWorldImporter> importer = _worldFormatManager.findImporterForFile(path);

	if (!importer)
		throw std::runtime_error("Can't find suitable importer for '" + path.string() + '\'');

	std::shared_ptr<World> world = importer->load(path, _gameManager, _worldFactoryManager);

	world->setExporter(_worldFormatManager.findAssociatedExporter(importer));
	world->setPath(path);

	_worlds.push_back(world);

	onWorldOpen(world, path);
	onWorldFileOperation(world, path);

	if (world->levels().empty())
		selectWorld(world);
	else
		selectLevel(world->levels().front());

	return world;
}

void BaseEditor::saveWorld(std::shared_ptr<World> world) {
	std::shared_ptr<IWorldExporter> exporter = world->exporter();

	if (!exporter)
		return;

	exporter->save(*world, world->path());

	onWorldSave(world, world->path());
	onWorldFileOperation(world, world->path());
}

void BaseEditor::saveWorldAs(
		std::shared_ptr<World> world, const fs::path &path, std::shared_ptr<IWorldExporter> exporter) {
	if (exporter)
		world->setExporter(exporter);

	if (!world->exporter())
		return;

	world->setPath(path);

	saveWorld(world);
}

void BaseEditor::closeWorld(std::shared_ptr<World> world) {
	auto it = std::find(_worlds.begin(), _worlds.end(), world);

	if (it == _worlds.end())
		return;

	_worlds.erase(it);

	if (selectedWorld() == world)
		selectLastWorld();
}

void BaseEditor::saveSelectedWorld() {
	std::shared_ptr<World> world = selectedWorld();

	if (!world)
		return;

	saveWorld(world);
}

void BaseEditor::saveSelectedWorldAs(const fs::path &path, std::shared_ptr<IWorldExporter> exporter) {
	std::shared_ptr<World> world = selectedWorld();

	if (!world)
		return;

	saveWorldAs(world, path, std::move(exporter));
}

void BaseEditor::closeSelectedWorld() {
	std::shared_ptr<World> world = selectedWorld();

	if (!world)
		return;

	closeWorld(world);
}

void BaseEditor::selectLastWorld() {
	if (_worlds.empty()) {
		selectWorld({});
		return;
	}

	selectFirstLevelOfWorld(_worlds.back());
}

std::shared_ptr<World> BaseEditor::selectedWorld() const {
	return _selectedWorld.lock();
}

std::shared_ptr<Level> BaseEditor::selectedLevel() const {
	return _selectedLevel.lock();
}

void BaseEditor::selectWorld(std::shared_ptr<World> world) {
	_selectedWorld = world;
	_selectedLevel.reset();
	onWorldSelectionChange(world);
	onLevelSelectionChange({});
}

void BaseEditor::selectFirstLevelOfWorld(std::shared_ptr<World> world) {
	_selectedWorld = world;

	if (world->levels().empty())
		_selectedLevel.reset();
	else
		_selectedLevel = world->levels().front();

	onWorldSelectionChange(world);
	onLevelSelectionChange(_selectedLevel.lock());
}

void BaseEditor::selectLevel(std::shared_ptr<Level> level) {
	_selectedWorld = level->world();
	_selectedLevel = level;
	onWorldSelectionChange(level->world());
	onLevelSelectionChange(level);
}

bool BaseEditor::hasSelectedWorld() const {
	return !_selectedWorld.expired();
}

void BaseEditor::onWorldSelectionChange(std::shared_ptr<World> world) {}

void BaseEditor::onLevelSelectionChange(std::shared_ptr<Level> level) {}

void BaseEditor::onWorldOpen(std::shared_ptr<World> world, const std::filesystem::path &path) {}

void BaseEditor::onWorldSave(std::shared_ptr<World> world, const std::filesystem::path &path) {}

void BaseEditor::onWorldFileOperation(std::shared_ptr<World> world, const std::filesystem::path &path) {}