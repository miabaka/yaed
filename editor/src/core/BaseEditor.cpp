#include "BaseEditor.hpp"

#include <stdexcept>
#include <utility>

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
	for (auto &[_, pFormat]: _worldFormats) {
		for (auto &pImporter: pFormat->importers()) {
			if (pImporter->probe(path))
				return pImporter;
		}
	}

	return {};
}

std::shared_ptr<World> BaseEditor::openWorld(const std::filesystem::path &path) {
	std::shared_ptr<IWorldImporter> importer = findImporterForFile(path);

	if (!importer)
		throw std::runtime_error("Can't find suitable importer for '" + path.string() + '\'');

	return importer->import(path, _gameManager);
}