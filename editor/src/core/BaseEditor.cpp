#include "BaseEditor.hpp"

#include <stdexcept>
#include <algorithm>

namespace fs = std::filesystem;

struct EditorWorldData : public ICustomData {
	std::shared_ptr<IWorldExporter> exporter;
};

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

const std::vector<std::shared_ptr<World>> &BaseEditor::worlds() const {
	return _worlds;
}

std::shared_ptr<World> BaseEditor::createWorld(const std::string &factoryId, const std::string &name) {
	std::shared_ptr<World> world =
			_worldFactoryManager.findFactoryById(factoryId)->createWorld(_gameManager, name);

	auto customData = std::make_unique<EditorWorldData>();
	world->setCustomData(std::move(customData));

	return world;
}

std::shared_ptr<World> BaseEditor::openWorld(const std::filesystem::path &path) {
	std::shared_ptr<IWorldImporter> importer = _worldFormatManager.findImporterForFile(path);

	if (!importer)
		throw std::runtime_error("Can't find suitable importer for '" + path.string() + '\'');

	std::shared_ptr<World> world = importer->load(path, _gameManager, _worldFactoryManager);

	auto customData = std::make_unique<EditorWorldData>();

	customData->exporter = _worldFormatManager.findAssociatedExporter(importer);

	world->setCustomData(std::move(customData));
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
	auto &customData = world->customData<EditorWorldData>();

	if (!customData.exporter)
		return;

	customData.exporter->save(*world, world->path());

	onWorldSave(world, world->path());
	onWorldFileOperation(world, world->path());
}

void BaseEditor::saveWorldAs(
		std::shared_ptr<World> world, const fs::path &path, std::shared_ptr<IWorldExporter> exporter) {
	auto &customData = world->customData<EditorWorldData>();

	if (exporter)
		customData.exporter = exporter;

	if (!customData.exporter)
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

	std::shared_ptr<World> world = _worlds.back();

	if (world->levels().empty())
		selectWorld(world);
	else
		selectLevel(world->levels().front());
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