#include "BaseEditor.hpp"

#include <stdexcept>

WorldFormat &BaseEditor::registerWorldFormat(const std::string &id, const WorldFormatInfo &formatInfo) {
	auto format = std::make_shared<WorldFormat>(formatInfo);

	bool inserted = _worldFormats.insert({id, format}).second;

	if (!inserted)
		throw std::runtime_error("World format with id '" + id + "' is already registered");

	return *format;
}

void BaseEditor::registerGame(std::shared_ptr<IGame> game) {
	_gameManager.registerGame(std::move(game));
}

std::shared_ptr<IWorldImporter> BaseEditor::findImporterForFile(const std::filesystem::path &path) const {
	for (auto &[_, format]: _worldFormats) {
		std::shared_ptr<IWorldImporter> importer = format->importer();

		if (!importer)
			continue;

		if (importer->probe(path))
			return importer;
	}

	return {};
}

std::set<std::shared_ptr<BaseWorldExporter>>
BaseEditor::findExportersForWorld(std::shared_ptr<const World> world) const {
	const std::string &gameId = world->game()->id();

	std::set<std::shared_ptr<BaseWorldExporter>> result;

	for (auto &[_, format]: _worldFormats) {
		std::shared_ptr<BaseWorldExporter> exporter = format->exporter();

		if (!(exporter && exporter->gameIsSupported(gameId)))
			continue;

		result.insert(exporter);
	}

	return result;
}

std::shared_ptr<World> BaseEditor::openWorld(const std::filesystem::path &path) {
	std::shared_ptr<IWorldImporter> importer = findImporterForFile(path);

	if (!importer)
		throw std::runtime_error("Can't find suitable importer for '" + path.string() + '\'');

	return importer->load(path, _gameManager);
}

void BaseEditor::saveWorldAs(std::shared_ptr<const World> world, const std::filesystem::path &path) const {

}