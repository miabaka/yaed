#include "BaseEditor.hpp"

#include <stdexcept>
#include <algorithm>

namespace fs = std::filesystem;

struct EditorWorldData : public ICustomData {
	fs::path path;
	std::shared_ptr<IWorldExporter> exporter;
};

GameManager &BaseEditor::gameManager() {
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

	customData->path = path;
	customData->exporter = _worldFormatManager.findAssociatedExporter(importer);

	world->setCustomData(std::move(customData));
	_worlds.push_back(world);

	return world;
}

void BaseEditor::saveWorld(std::shared_ptr<const World> world) const {
	auto &customData = world->customData<EditorWorldData>();

	if (!customData.exporter)
		return;

	customData.exporter->save(*world, customData.path);
}

void BaseEditor::saveWorldAs(
		std::shared_ptr<World> world, const std::filesystem::path &path,
		std::shared_ptr<IWorldExporter> exporter) const {
	auto &customData = world->customData<EditorWorldData>();

	if (exporter)
		customData.exporter = exporter;

	if (!customData.exporter)
		return;

	customData.exporter->save(*world, path);
	customData.path = path;
}

void BaseEditor::closeWorld(std::shared_ptr<World> world) {
	auto it = std::find(_worlds.begin(), _worlds.end(), world);

	if (it == _worlds.end())
		return;

	_worlds.erase(it);

	if (selectedWorld() == world)
		selectWorld({});
}

void BaseEditor::saveSelectedWorld() const {
	std::shared_ptr<World> world = selectedWorld();

	if (!world)
		return;

	saveWorld(world);
}

void BaseEditor::saveSelectedWorldAs(const fs::path &path, std::shared_ptr<IWorldExporter> exporter) const {
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

std::shared_ptr<World> BaseEditor::selectedWorld() const {
	return _selectedWorld.lock();
}

std::shared_ptr<Level> BaseEditor::selectedLevel() const {
	return _selectedLevel.lock();
}

void BaseEditor::selectWorld(std::shared_ptr<World> world) {
	_selectedWorld = world;
	_selectedLevel.reset();
	onLevelSelectionChange({});
}

void BaseEditor::selectLevel(std::shared_ptr<Level> level) {
	_selectedWorld = level->world();
	_selectedLevel = level;
	onLevelSelectionChange(level);
}

void BaseEditor::onLevelSelectionChange(std::shared_ptr<Level> level) {}