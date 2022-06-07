#include "WorldFormatManager.hpp"

WorldFormat &WorldFormatManager::registerFormat(const std::string &id, const WorldFormatInfo &formatInfo) {
	auto format = std::make_shared<WorldFormat>(formatInfo);

	bool inserted = _worldFormats.insert({id, format}).second;

	if (!inserted)
		throw std::runtime_error("World format with id '" + id + "' is already registered");

	return *format;
}

std::shared_ptr<IWorldImporter> WorldFormatManager::findImporterForFile(const std::filesystem::path &path) const {
	for (auto &[_, format]: _worldFormats) {
		std::shared_ptr<IWorldImporter> importer = format->importer();

		if (!importer)
			continue;

		if (importer->probe(path))
			return importer;
	}

	return {};
}

std::set<std::shared_ptr<IWorldExporter>>
WorldFormatManager::findExportersForWorld(std::shared_ptr<const World> world) const {
	const std::string &gameId = world->game()->id();

	std::set<std::shared_ptr<IWorldExporter>> result;

	for (auto &[_, format]: _worldFormats) {
		std::shared_ptr<IWorldExporter> exporter = format->exporter();

		if (!(exporter && exporter->gameIsSupported(gameId)))
			continue;

		result.insert(exporter);
	}

	return result;
}

std::shared_ptr<IWorldExporter>
WorldFormatManager::findAssociatedExporter(std::shared_ptr<const IWorldImporter> importer) const {
	for (auto &[_, format]: _worldFormats) {
		std::shared_ptr<IWorldImporter> currentImporter = format->importer();

		if (!(currentImporter && currentImporter == importer))
			continue;

		return format->exporter();
	}

	return {};
}

const std::unordered_map<std::string, std::shared_ptr<WorldFormat>> &WorldFormatManager::formats() const {
	return _worldFormats;
}