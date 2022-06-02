#include "BaseEditor.hpp"

#include <stdexcept>

namespace fs = std::filesystem;

struct EditorWorldData : public ICustomWorldData {
	fs::path path;
	std::shared_ptr<BaseWorldExporter> exporter;
};

GameManager &BaseEditor::gameManager() {
	return _gameManager;
}

WorldFormatManager &BaseEditor::worldFormatManager() {
	return _worldFormatManager;
}

std::shared_ptr<World> BaseEditor::openWorld(const std::filesystem::path &path) {
	std::shared_ptr<IWorldImporter> importer = _worldFormatManager.findImporterForFile(path);

	if (!importer)
		throw std::runtime_error("Can't find suitable importer for '" + path.string() + '\'');

	std::shared_ptr<World> world = importer->load(path, _gameManager);

	auto customData = std::make_unique<EditorWorldData>();

	customData->path = path;
	customData->exporter = _worldFormatManager.findAssociatedExporter(importer);

	world->setCustomData(std::move(customData));

	return world;
}

void BaseEditor::saveWorldAs(std::shared_ptr<const World> world, const std::filesystem::path &path) const {
	auto &customData = world->customData<EditorWorldData>();

	if (!customData.exporter)
		return;

	customData.exporter->save(*world, path);
}