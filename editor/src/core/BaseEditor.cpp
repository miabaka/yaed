#include "BaseEditor.hpp"

#include <stdexcept>

namespace fs = std::filesystem;

struct EditorWorldData : public ICustomData {
	fs::path path;
	std::shared_ptr<BaseWorldExporter> exporter;
};

GameManager &BaseEditor::gameManager() {
	return _gameManager;
}

WorldFormatManager &BaseEditor::worldFormatManager() {
	return _worldFormatManager;
}

WorldFactoryManager &BaseEditor::worldFactoryManager() {
	return _worldFactoryManager;
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

	_worlds.insert(world);

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
		std::shared_ptr<BaseWorldExporter> exporter) const {
	auto &customData = world->customData<EditorWorldData>();

	if (exporter)
		customData.exporter = exporter;

	if (!customData.exporter)
		return;

	customData.exporter->save(*world, path);
	customData.path = path;
}

void BaseEditor::closeWorld(std::shared_ptr<World> world) {
	auto it = _worlds.find(world);

	if (it == _worlds.end())
		return;

	_worlds.erase(it);
}